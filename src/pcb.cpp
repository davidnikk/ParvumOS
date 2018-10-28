#include "pcb.h"
#include "schedule.h"
#include <dos.h>
#include "list.h"
#include "idle.h"
#include "semlist.h"
#include <assert.h>
#include "sortlist.h"

void tick();

int PCB::timer_from_dispatch = 0;
PCB* PCB::running = 0;
PCB* PCB::idle = 0;
Time PCB::runningUptime = 0;
pInterrupt PCB::intr08 = 0;
SemList* PCB::semList = 0;
ID PCB::idNext = 0;
List* PCB::listOfAllThreads = 0;
SortList* PCB::listOfSLeepingThreads = 0;

ID PCB::getId()
{
	return id;
}

ID PCB::getRunningId()
{
	return running->id;
}

Thread* PCB::getThreadById(ID id)
{
	return listOfAllThreads->getThreadById(id);
}

void PCB::addPCB(PCB* pcb)
{
	listOfAllThreads->put(pcb);
}

void PCB::removePCB(PCB* pcb)
{
	listOfAllThreads->remove(pcb);
}

void PCB::init()
{
	semList = new SemList();
	listOfAllThreads = new List();

	Thread* t = new Thread();
	running = t->myPCB;

	Idle* i = new Idle();
	idle = i->myPCB;
	i->start();

	LOCK
#ifndef BCC_BLOCK_IGNORE
	intr08 = getvect(0x08);
	setvect(0x08, (pInterrupt)PCB::timer);
#endif
	UNLOCK
}

void PCB::shutdown()
{
	LOCK
#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, intr08);
#endif
	UNLOCK

	Idle* i = (Idle*)idle->thread;
	delete i;
	idle = 0;

	Thread* t = running->thread;
	delete t;
	running = 0;

	delete listOfAllThreads;
	delete semList;
}

void interrupt PCB::timer()
{

	static int do_dispatch = 0;

	if (timer_from_dispatch)
		do_dispatch = 1;
	else
		do_dispatch = 0;

	if (!timer_from_dispatch)
	{

		intr08();
		tick();

		if (running->timeSlice > 0)
		{
			runningUptime++;
			if (runningUptime >= running->timeSlice)
			{
				do_dispatch = 1;
			}
		}

		listOfSLeepingThreads->update();
	}

	if (do_dispatch)
	{
		static unsigned tss = 0;
		static unsigned tsp = 0;
		static unsigned tbp = 0;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tss, ss
			mov tsp, sp
			mov tbp, bp
		}
#endif
		running->ss = tss;
		running->sp = tsp;
		running->bp = tbp;


		if ((idle != running) && (running->status == READY))
			Scheduler::put(running);
		running = Scheduler::get();
		if (running == 0)
		{
			running = idle;
		}

		tss = running->ss;
		tsp = running->sp;
		tbp = running->bp;
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov ss, tss
			mov sp, tsp
			mov bp, tbp
		}
#endif



		runningUptime = 0;
	}

	timer_from_dispatch = 0;


}

void PCB::dispatch()
{
	LOCK
	PCB::timer_from_dispatch = 1;
	PCB::timer();
	UNLOCK
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread)
	: stackSize(stackSize)
	, timeSlice(timeSlice)
	, ss(0)
	, sp(0)
	, bp(0)
	, thread(thread)
	, stack(0)
	, status(NEW)
	, completingList(0)
	, semWaitResult(0)
	, id(idNext++)
{
	completingList = new List();
}

void PCB::start()
{
	if (status != NEW)
		return;

	status = READY;
	if (this != idle)
	{
		Scheduler::put(this);
	}
	initStartingStack();
}

void PCB::waitToComplete()
{
	if (status == COMPLETED)
		return;
	if (running == this)
		return;
	if (status == NEW)
		return;
	if (this == idle)
		return;

	running->status = BLOCKED;
	completingList->put(running);
	dispatch();
}

void PCB::run()
{
	if (running->thread)
		running->thread->run();

	while (running->completingList->getSize() > 0)
	{
		PCB* pcb = running->completingList->getFirst();
		pcb->status = READY;
		Scheduler::put(pcb);
	}
	running->status = COMPLETED;
	dispatch();
}

void PCB::initStartingStack()
{
	StackSize size = stackSize / sizeof(unsigned);
	stack = new unsigned[size];
	stack[size - 1] = 512; 
#ifndef BCC_BLOCK_IGNORE
	stack[size - 2] = FP_SEG(PCB::run); 
	stack[size - 3] = FP_OFF(PCB::run);

	ss = FP_SEG(stack + size - 12);
	sp = FP_OFF(stack + size - 12);
	bp = FP_OFF(stack + size - 12);
#endif
}

PCB::~PCB()
{
	delete completingList;
	delete[] stack;
}

void PCB::sleep(Time timeToSleep)
{
	if (timeToSleep == 0)
		return;

	running->status = BLOCKED;
	listOfSLeepingThreads->put(running, timeToSleep);
	dispatch();
}
