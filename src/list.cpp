#include "list.h"
#include "pcb.h"


List::ListElem::ListElem(PCB* pcb)
{
	this->pcb = pcb;
	next = 0;
}

List::List()
{
	size = 0;
	head = tail = 0;
}

List::~List()
{
	ListElem* tmp = head;
	while (tmp)
	{
		head = tmp->next;
		delete tmp;
		tmp = head;
	}
}

void List::put(PCB* pcb)
{
	if (head == 0)
	{
		head = new ListElem(pcb);
		tail = head;
	}
	else
	{
		tail->next = new ListElem(pcb);
		tail = tail->next;
	}
	size++;
}

PCB* List::getFirst()
{
	if (head == 0)
		return 0;

	ListElem* tmp = head;
	head = tmp->next;
	PCB* p = tmp->pcb;
	if (tmp->next == 0)
		tail = 0;
	delete tmp;
	size--;
	return p;
}

Thread* List::getThreadById(ID id)
{
	if (head == 0)
		return 0;

	ListElem* tmp = head;
	while (tmp != 0 && tmp->pcb->id != id)
	{
		tmp = tmp->next;
	}

	if (tmp == 0)
		return 0;

	return tmp->pcb->thread;
}

void List::remove(PCB* pcb)
{
	ListElem *tmp = head, *tmp2 = 0;
	while (tmp)
	{
		if (tmp->pcb == pcb)
		{
			if (tmp2)
				tmp2->next = tmp->next;
			else
				head = tmp->next;

			if (tmp->next == 0)
			{
				tail = tmp2;
			}
			size--;
			return;
		}

		tmp2 = tmp;
		tmp = tmp->next;
	}
}

int List::getSize()
{
	return size;
}


