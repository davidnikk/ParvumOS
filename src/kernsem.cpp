#include "kernsem.h"
#include "pcb.h"
#include "list.h"
#include "schedule.h"
#include "semlist.h"


KernelSem::KernelSem(int init)
	: value(init)
	, blockedList(0)
{
	blockedList = new List();
}

KernelSem::~KernelSem()
{
	while (val() < 0)
		signal();
	delete blockedList;
}

int KernelSem::wait(int toBlock)
{
	if (toBlock)
	{
		if (--value < 0)
		{
			PCB::running->semWaitResult = 1;
			PCB::running->status = BLOCKED;
			blockedList->put(PCB::running);
			dispatch();
		}
		else
		{
			PCB::running->semWaitResult = 0;
		}

		return PCB::running->semWaitResult;
	}
	else
	{
		if (value > 0)
		{
			--value;
			PCB::running->semWaitResult = 0;
		}
		else
		{
			PCB::running->semWaitResult = -1;
		}
	}

	return PCB::running->semWaitResult;
}

void KernelSem::signal()
{
	if (value++ < 0)
	{
		if (blockedList->getSize() > 0)
		{
			PCB* pcb = blockedList->getFirst();
			pcb->status = READY;
			Scheduler::put(pcb);
		}
	}
}
