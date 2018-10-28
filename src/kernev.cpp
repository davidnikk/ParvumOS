#include "kernev.h"
#include "pcb.h"
#include "schedule.h"
#include "ivtentry.h"

#define BLOCKED_EVENT 1
#define SIGNAL_EVENT 2
#define READY_EVENT 3

KernelEv::KernelEv(IVTNo ivtNo)
{
	pcb = PCB::running;
	status = READY_EVENT;
	this->ivtNo = ivtNo;
	IVTEntry::ivtEntryArray[ivtNo]->event = this;
}

KernelEv::~KernelEv()
{
	signal();
	IVTEntry::ivtEntryArray[ivtNo]->event = 0;
}

void KernelEv::signal()
{
	if (status == BLOCKED_EVENT)
	{
		status = READY_EVENT;
		pcb->status = READY;
		Scheduler::put(pcb);
		return;
	}
	else if (status == READY_EVENT)
	{
		status = SIGNAL_EVENT;
		return;
	}
}

void KernelEv::wait()
{
	if (PCB::running != pcb)
		return;

	if (status == SIGNAL_EVENT)
	{
		status = READY_EVENT;
		return;
	}
	else if (status == READY_EVENT)
	{
		status = BLOCKED_EVENT;
		pcb->status = BLOCKED;
		dispatch();
		return;
	}
}

