#include <stdlib.h>
#include "pcb.h"
#include "PCBList.h"

unsigned PCB::ID = 0;
PCB* PCB::running = 0;
PCBList PCB::ready;
Thread* PCB::idle = 0;
PCB* PCB::main = 0;
volatile unsigned PCB::lockFlag = 1;
int PCB::blockedGlobally[16] = { 0 };

PCB::PCB(StackSize stackSize, Time time, Thread *thread) {
	myThread = thread;
	if (stackSize > MAX_STACK_SIZE) stackSize = MAX_STACK_SIZE;
	stackSize = stackSize / 2 + (stackSize & 1);
	stack = new unsigned[stackSize];
	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2] = FP_SEG(&wrapper);
	stack[stackSize - 3] = FP_OFF(&wrapper);
	sp = FP_OFF(stack + stackSize - 12);
	ss = FP_SEG(stack + stackSize - 12);
	bp = sp;
	state = NEW;
	timeSlice = time;
	id = ++ID;
	for(int i = 0; i < 16; i++) {
		blocked[i] = 0;
		handlers[i] = 0;
	}
	nasledi(parent = PCB::running);
	firstSign = lastSign = 0;
}

void PCB::nasledi(PCB *parent) {
	if(!parent) return;
	for (unsigned i = 0; i < 16; i++) {
			if (!parent->handlers[i]) continue;
			SignalHandlerNode *node = parent->handlers[i]->next, *start = node;
			registerHandler(i, node->handler);
			node = node->next;
			while (start != node) {
				registerHandler(i, node->handler);
				node = node->next;
			}
		}
}

void PCB::wrapper() {
	running->myThread->run();
	lockCout
	running->state = FINISHED;
	PCB::running->waitQueue.removeAll();
	unlockCout
	dispatch();
}

void PCB::start() {
	lock
	state = READY;
	ready.add(this, 0);
	if(stack) Scheduler::put(this);
	else state = FINISHED;
	unlock
}

void PCB::dispatch() {
	Timer::dispatch();
}

PCB::~PCB() {
	lock
	delete [] stack;
	myThread = 0;
	for(unsigned i = 0; i < 16; i++) unregisterAllHandlers(i);
	while(firstSign) {
		SignalNode *old = firstSign;
		firstSign = firstSign->next;
		delete old;
	}
	lastSign = 0;
	unlock
}

void PCB::createMainPCB() {
	lock
	running = main = new PCB(defaultStackSize, defaultTimeSlice, 0);
	running->state = READY;
	running->parent = 0;
	ready.add(PCB::running, 0);
	idle = new Idle;
	cout << "Created Idle with id: " << idle->myPCB->id << endl;
	idle->myPCB->state = BLOCKED;
	ready.add(idle->myPCB, 0);
	unlock
}

void PCB::deleteMainPCB() {
	lock
	delete running;
	running = 0;
	delete idle;
	idle = 0;
	unlock
}

ID PCB::getRunningId() {
	return running ? running->id : 0;
}


void PCB::signal(SignalId signal) {
	lockCout
		lastSign = (firstSign ? lastSign->next : firstSign) = new SignalNode(signal);
	unlockCout
}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	if (signal >= 0 && signal <= 15) {
		lock
		SignalHandlerNode *newNode = new SignalHandlerNode(handler);
		unlock;
		lockCout
		if (handlers[signal]) {
			newNode->next = handlers[signal]->next;
			handlers[signal]->next = newNode;
			handlers[signal] = handlers[signal]->next;
		}
		else {
			handlers[signal] = newNode;
			newNode->next = newNode;
		}
		unlockCout
	}
}

void PCB::unregisterAllHandlers(SignalId id) {
	lockCout
		if (handlers[id]) {
			SignalHandlerNode *node = handlers[id]->next, *old;
			handlers[id]->next = 0;
			while (node) {
				old = node;
				node = node->next;
				lock
				delete old;
				unlock
			}
			handlers[id] = 0;
		}
	unlockCout
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	lockCout
		if (id >= 0 && id <= 15 && handlers[id] && hand1 != hand2) {
			SignalHandlerNode *tmp = handlers[id], *begin = tmp, *hn1 = 0, *hn2 = 0;
			if (tmp->handler == hand1) hn1 = tmp;
			else if (tmp->handler == hand2) hn2 = tmp;
			tmp = tmp->next;
			while (tmp != begin && hn1 == 0 && hn2 == 0) {
				if (tmp->handler == hand1) hn1 = tmp;
				else if (tmp->handler == hand2) hn2 = tmp;
				tmp = tmp->next;
			}
			if (tmp == begin) {
				unlockCout
				return;
			}
			while (tmp != begin && hn1 == 0) {
				if (tmp->handler == hand1) hn1 = tmp;
				tmp = tmp->next;
			}
			while (tmp != begin && hn2 == 0) {
				if (tmp->handler == hand2) hn2 = tmp;
				tmp = tmp->next;
			}
			if (hn1 == 0 || hn2 == 0) {
				unlockCout
				return;
			}
			SignalHandler h = hn1->handler;
			hn1->handler = hn2->handler;
			hn2->handler = h;
		}
	unlockCout
}

void PCB::blockSignal(SignalId signal) {
	lockCout
		blocked[signal] = 1;
	unlockCout
}
void PCB::blockSignalGlobally(SignalId signal) {
	lockCout
		blockedGlobally[signal] = 1;
	unlockCout
}
void PCB::unblockSignal(SignalId signal) {
	lockCout
		blocked[signal] = 0;
	unlockCout
}

void PCB::unblockSignalGlobally(SignalId signal) {
	lockCout
		blockedGlobally[signal] = 0;
	unlockCout
}

void PCB::obradi() {
	lockCout
	asm {
		pushf
		push ax
		mov ax, 200h
		push ax
		popf
		pop ax
	}
	SignalHandlerNode *node, *end;
		SignalNode *old, *prev = 0, *tmp = firstSign;
		while (tmp) {
			node = handlers[tmp->i];
			if (!blockedGlobally[tmp->i] && !blocked[tmp->i]) {
				if (node) {
					end = node = node->next;
					node->handler();
					node = node->next;
					while (node != end) {
						node->handler();
						node = node->next;
					}
				}
				old = tmp;
				if (old == firstSign) firstSign = firstSign->next;
				tmp = tmp->next;
				if (prev) prev->next = tmp;
				if (old == lastSign) lastSign = prev;
				delete old;
			}
			else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
		if (!firstSign) lastSign = 0;
	asm popf;
	unlockCout
}
