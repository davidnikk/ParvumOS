#include "thread.h"
#include "pcb.h"


void dispatch()
{
	LOCK
	PCB::dispatch();
	UNLOCK
}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	LOCK
	myPCB = new PCB(stackSize, timeSlice, this);
	PCB::addPCB(myPCB);
	UNLOCK
}

Thread::~Thread()
{
	waitToComplete();
	LOCK
	PCB::removePCB(myPCB);
	delete myPCB;
	UNLOCK
}


void Thread::start()
{
	LOCK
	myPCB->start();
	UNLOCK
}

void Thread::waitToComplete()
{
	LOCK
	myPCB->waitToComplete();
	UNLOCK
}

void Thread::sleep(Time timeToSleep)
{
	LOCK
	PCB::sleep(timeToSleep);
	UNLOCK
}
