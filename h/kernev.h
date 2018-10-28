#ifndef H_KERNEV_H_
#define H_KERNEV_H_

typedef unsigned char IVTNo;
class PCB;

class KernelEv
{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void signal();
	void wait();
private:
	PCB* pcb;
	int status;
	IVTNo ivtNo;
};


#endif /* H_KERNEV_H_ */
