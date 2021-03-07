#ifndef _timer_h_
#define _timer_h_

#include "pcb.h"
#include "SCHEDULE.h"
#include <iostream.h>
#include <dos.h>
#include "kernlsem.h";

class Timer {
public:
	static void inic();
	static void restore();
	static void interrupt myRoutine();
	static void dispatch();
//private:
	static int cnt, dmnd;
};

#endif
