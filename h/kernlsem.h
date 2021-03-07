#ifndef _kernlsem_h_
#define _kernlsem_h_

#include "PCBList.h"
#include "SemList.h"
typedef unsigned Time;

class KernelSem {
public:
	KernelSem(int init);
	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n);

	int val() const;

	virtual ~KernelSem();
//private:
	friend class Semaphore;
	friend class SemList;
	friend class Timer;
	PCBList blocked;
	int value;
	static SemList semList;
};

#endif
