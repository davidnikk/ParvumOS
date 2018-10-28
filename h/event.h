#ifndef H_EVENT_H_
#define H_EVENT_H_

#include "prepare.h"
#include "ivtentry.h"

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal();
private:
	KernelEv* myImpl;
};


#endif
