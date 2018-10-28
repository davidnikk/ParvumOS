#ifndef H_SORTLIST_H_
#define H_SORTLIST_H_


class PCB;
typedef unsigned int Time;

#define INVALID_TIME 0xffff

class SortList
{
public:
	SortList();
	~SortList();
	void put(PCB* pcb, Time time);
	Time getTimeOfFirstFinite();
	void update();
	PCB* getFirst();
	int getSize();
private:
	struct ListElem
	{
		int isFinite;
		Time time;
		PCB* pcb;
		ListElem* next;
		ListElem(Time time, PCB* pcb, int isFinite);
	};
	ListElem *head;
	ListElem *firstFinite;
	int size;
};


#endif /* H_SORTLIST_H_ */
