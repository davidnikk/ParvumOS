#ifndef H_PREPARE_H_
#define H_PREPARE_H_

#include <iostream.h>

#define PREPAREENTRY(ivtNo, callPrevRoutine)\
void interrupt routine##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo, routine##ivtNo);\
void interrupt routine##ivtNo(...)\
{\
	entry##ivtNo.signal(callPrevRoutine);\
}

#endif /* H_PREPARE_H_ */
