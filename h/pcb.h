#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"

class List;
class SemList;
class SortList;

typedef void interrupt (*pInterrupt)(...);

#define READY 1
#define BLOCKED 2
#define NEW 3
#define COMPLETED 4

#ifdef BCC_BLOCK_IGNORE
#define LOCK
#define UNLOCK
#else
#define LOCK asm { pushf; cli; }
#define UNLOCK asm popf;
#endif

class PCB
{
public:
	PCB(StackSize stackSize, Time timeSlice, Thread* thread);
	static void init();
	static void shutdown();
	static void interrupt timer();
	static void dispatch();
	void start();
	void waitToComplete();
	static void run();
	void initStartingStack();
	~PCB();
	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);
	static void addPCB(PCB* pcb);
	static void removePCB(PCB* pcb);
	static void sleep(Time timeToSleep);
private:
	friend class KernelSem;
	friend class List;
	friend class SortList;
	friend class KernelEv;
	static int timer_from_dispatch;
	static PCB* running;
	static PCB* idle;
	StackSize stackSize;
	Time timeSlice;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	Thread* thread;
	unsigned* stack;
	int status;
	List* completingList;
	static Time runningUptime;
	static pInterrupt intr08;
	int semWaitResult;
	static SemList* semList;
	static ID idNext;
	ID id;
	static List* listOfAllThreads;
	static SortList* listOfSLeepingThreads;
};

#endif
