#ifndef H_LIST_H_
#define H_LIST_H_

typedef int ID;
class PCB;
class Thread;

class List
{
public:
	List();
	~List();
	void put(PCB* pcb);
	PCB* getFirst();
	Thread* getThreadById(ID id);
	void remove(PCB* pcb);
	int getSize();
private:
	struct ListElem
	{
		PCB* pcb;
		ListElem* next;
		ListElem(PCB* pcb);
	};
	ListElem *head, *tail;
	int size;
};


#endif /* H_LIST_H_ */
