#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);
class KernelEv;

class IVTEntry
{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
	void signal(int callPrev);
	~IVTEntry();
private:
	friend class KernelEv;
	IVTNo ivtNo;
	pInterrupt newRoutine;
	pInterrupt prevRoutine;
	KernelEv* event;
	static IVTEntry* ivtEntryArray[256];
};


#endif /* H_IVTENTRY_H_ */
