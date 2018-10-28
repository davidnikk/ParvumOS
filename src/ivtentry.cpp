#include "ivtentry.h"
#include "kernev.h"
#include "pcb.h"
#include <dos.h>

IVTEntry* IVTEntry::ivtEntryArray[256]={0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine)
{
	this->ivtNo = ivtNo;
	this->newRoutine = newRoutine;
	prevRoutine = 0;
	event = 0;
	LOCK
#ifndef BCC_BLOCK_IGNORE
	prevRoutine = getvect(ivtNo);
	setvect(ivtNo, (pInterrupt)newRoutine);
#endif
	ivtEntryArray[ivtNo] = this;
	UNLOCK
}

void IVTEntry::signal(int callPrev)
{
	if (event != 0)
		event->signal();
	if (prevRoutine != 0 && callPrev != 0)
		prevRoutine();
	dispatch();
}

IVTEntry::~IVTEntry()
{
	LOCK
	ivtEntryArray[ivtNo] = 0;
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, prevRoutine);
#endif
	if (prevRoutine != 0)
		prevRoutine();
	UNLOCK
}

