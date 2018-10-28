#include "schedule.h"
#include "sortlist.h"
#include "pcb.h"
#include <assert.h>
#include <iostream.h>

SortList::ListElem::ListElem(Time time, PCB* pcb, int isFinite)
{
	this->time = time;
	this->pcb = pcb;
	this->isFinite = isFinite;
	next = 0;
}

SortList::SortList()
{
	size = 0;
	head = 0;
	firstFinite = 0;
}

SortList::~SortList()
{
	ListElem* tmp = head;
	while (tmp)
	{
		head = tmp->next;
		delete tmp;
		tmp = head;
		size--;
	}
}

void SortList::put(PCB* pcb, Time time)
{
	int isFinite = time != 0;

	ListElem* cur = head;
	ListElem* prev = 0;

	int found = 0;
	while ((!found) && (cur != 0))
	{
		if (time >= cur->time)
		{
			time = time - cur->time;
			prev = cur;
			cur = cur->next;
		}
		else
		{
			found = 1;
		}
	}

	if (!found)
	{
		if (prev == 0)
		{
			assert(head == 0);
			head = new ListElem(time, pcb, isFinite);
			if (isFinite)
			{
				firstFinite = head;
			}
		}
		else
		{
			prev->next = new ListElem(time, pcb, isFinite);
			if (!prev->isFinite && isFinite)
			{
				firstFinite = prev->next;
			}
		}
	}
	else
	{
		if (prev == 0)
		{
			ListElem* elem = new ListElem(time, pcb, isFinite);
			elem->next = head;
			head = elem;
			if (isFinite)
			{
				firstFinite = elem;
			}
		}
		else
		{
			ListElem* elem = new ListElem(time, pcb, isFinite);
			elem->next = cur;
			prev->next = elem;
			if (!prev->isFinite && isFinite)
			{
				firstFinite = elem;
			}
		}

		cur->time = cur->time - time;
	}

	size++;


}

Time SortList::getTimeOfFirstFinite()
{
	if (firstFinite == 0)
	{
		return INVALID_TIME;
	}

	return firstFinite->time;
}

void SortList::update()
{
	if (firstFinite == 0)
	{
		return;
	}

	--firstFinite->time;

	if (firstFinite->time == 0)
	{
		ListElem* lastInfinite = 0;
		if (firstFinite != head)
		{
			lastInfinite = head;
			while (lastInfinite->next != firstFinite)
			{
				lastInfinite = lastInfinite->next;
			}
		}

		while (firstFinite != 0 && firstFinite->time == 0)
		{
			if (lastInfinite != 0)
			{
				lastInfinite->next = firstFinite->next;
			}
			else
			{
				head = firstFinite->next;
			}

			PCB* pcb = firstFinite->pcb;
			pcb->status = READY;
			Scheduler::put(pcb);
			delete firstFinite;
			size--;

			if (lastInfinite != 0)
			{
				firstFinite = lastInfinite->next;
			}
			else
			{
				firstFinite = head;
			}
		}
	}
}

PCB* SortList::getFirst()
{
	if (head == 0)
	{
		return 0;
	}

	ListElem* old = head;
	head = head->next;
	if (old == firstFinite)
	{
		firstFinite = head;
	}
	if (old->time > 0)
	{
		
		if (old->next != 0)
		{
			old->next->time = old->next->time + old->time;
		}
	}
	size--;

	PCB* pcb = old->pcb;
	delete old;
	return pcb;
}

int SortList::getSize()
{
	return size;
}


