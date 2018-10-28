#ifndef H_SEMLIST_H_
#define H_SEMLIST_H_


class KernelSem;

class SemList
{
public:
	SemList();
	~SemList();
	void put(KernelSem* sem);
	void remove(KernelSem* sem);
	void update();
private:
	struct ListElem
	{
		KernelSem* sem;
		ListElem* next;
		ListElem(KernelSem* sem);
	};
	ListElem *head;
};


#endif /* H_SEMLIST_H_ */
