#ifndef H_KERNSEM_H_
#define H_KERNSEM_H_

#include "semaphor.h"
class List;

class KernelSem
{
public:
	KernelSem(int init);
	~KernelSem();
	int wait(int toBlock);
	void signal();
	int val() const { return value; }
private:
	friend class SemList;
	int value;
	List* blockedList;
};


#endif /* H_KERNSEM_H_ */
