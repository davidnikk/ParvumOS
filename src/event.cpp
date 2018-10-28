#include "event.h"
#include "pcb.h"
#include "kernev.h"


Event::Event(IVTNo ivtNo)
{
	LOCK
	myImpl = new KernelEv(ivtNo);
	UNLOCK
}

Event::~Event()
{
	LOCK
	delete myImpl;
	UNLOCK
}

void Event::signal()
{
	LOCK
	myImpl->signal();
	UNLOCK
}

void Event::wait()
{
	LOCK
	myImpl->wait();
	UNLOCK
}

