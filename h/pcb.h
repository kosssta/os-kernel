#ifndef _pcb_h_
#define _pcb_h_

#include "idle.h"
#include "SCHEDULE.h"
#include "timer.h"
#include <dos.h>
#include <iostream.h>
#include "signals.h"
#include "PCBList.h"

#ifndef lock

#define lock asm {\
	pushf;\
	cli;\
}
#define unlock asm popf

#define lockCout PCB::lockFlag = 0;
#define unlockCout PCB::lockFlag = 1;\
	if (Timer::dmnd) dispatch();

#endif

const StackSize MAX_STACK_SIZE = 65356;

class PCB {
public:
	enum State { NEW, READY, BLOCKED, FINISHED };
	PCB (StackSize stackSize, Time time, Thread *thread);
	static ID getRunningId();
	void start();
	static void dispatch();
	virtual ~PCB();
	static void createMainPCB();
	static void deleteMainPCB();
	//PCBList* block;
	static PCBList ready;
	PCBList waitQueue;
	static Thread* idle;
	static PCB *main;
	static volatile unsigned lockFlag;

	SignalHandlerNode* handlers[16];
	SignalNode* firstSign, *lastSign;
	static int blockedGlobally[16];
	int blocked[16];

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
	void obradi();
	void nasledi(PCB *parent);
	PCB* parent;
//protected:
	friend class Thread;
	friend class Timer;
	friend class KernelSem;
	friend class PCBList;
	friend class Idle;
	static void wrapper();
	Thread *myThread;
	State state;
	unsigned* stack;
	unsigned ss, sp, bp;
	Time timeSlice;
	static unsigned ID;
	unsigned id;
	static PCB* running;
};

#endif
