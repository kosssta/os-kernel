#ifndef _idle_h_
#define _idle_h_

#include "thread.h"

class Idle : public Thread {
public:
	Idle() : acq(0) { }

protected:
	void run();

private:
	volatile int acq;
};

#endif
