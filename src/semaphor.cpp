#include "semaphor.h"
#include "pcb.h"
#include "kernsem.h"


Semaphore::Semaphore(int init)
{
	LOCK
	myImpl = new KernelSem(init);
	UNLOCK
}

Semaphore::~Semaphore()
{
	LOCK
	delete myImpl;
	UNLOCK
}

int Semaphore::wait(int toBlock)
{
	LOCK
	int res = myImpl->wait(toBlock);
	UNLOCK
	return res;
}

void Semaphore::signal()
{
	LOCK
	myImpl->signal();
	UNLOCK
}

int Semaphore::val() const
{
	LOCK
	int res = myImpl->val();
	UNLOCK
	return res;
}
