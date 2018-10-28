#include "semlist.h"
#include "kernsem.h"
#include "sortlist.h"


SemList::ListElem::ListElem(KernelSem* sem)
{
	this->sem = sem;
	next = 0;
}

SemList::SemList()
{
	head = 0;
}

SemList::~SemList()
{
	ListElem* tmp = head;
	while (tmp)
	{
		head = tmp->next;
		delete tmp;
		tmp = head;
	}
}

void SemList::put(KernelSem* sem)
{
	ListElem* elem = new ListElem(sem);
	elem->next = head;
	head = elem;
}

void SemList::remove(KernelSem* sem)
{
	ListElem* cur = head;
	ListElem* prev = 0;

	while ((cur != 0) && (cur->sem != sem))
	{
		prev = cur;
		cur = cur->next;
	}

	if (cur != 0)
	{
		if (prev != 0)
			prev->next = cur->next;
		else
			head = cur->next;

		delete cur;
	}
}

void SemList::update()
{
	ListElem* cur = head;
	while (cur != 0)
	{
		cur = cur->next;
	}
}

