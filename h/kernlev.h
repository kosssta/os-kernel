#ifndef _kernlev_h_
#define _kernlev_h_

typedef unsigned char IVTNo;

class IVTEntry;
class Event;
class PCB;
typedef unsigned char IVTNo;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	void signal();
	void wait();
	~KernelEv();
private:
	int value;
	IVTNo ivtNo;
	PCB *owner;
};

#endif
