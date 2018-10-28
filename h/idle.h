#ifndef H_IDLE_H_
#define H_IDLE_H_


#include "thread.h"

class Idle : public Thread
{
public:
	Idle() : Thread(200, 1) {}
	void run()
	{
		while (1);
	}
};


#endif /* H_IDLE_H_ */
