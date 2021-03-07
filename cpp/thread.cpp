#include "thread.h"
#include "pcb.h"
#include "PCBList.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

void Thread::start() {
	if (myPCB && myPCB->state == PCB::NEW) myPCB->start();
}

Thread::~Thread() {
	waitToComplete();
	lock
	delete myPCB;
	myPCB = 0;
	unlock
}

void Thread::waitToComplete() {
	if(myPCB && myPCB != PCB::running && this != PCB::idle && myPCB->state != PCB::NEW && myPCB->state != PCB::FINISHED) {
			//while(myPCB->state != PCB::FINISHED) {
			//dispatch();
		//}
		lockCout
		PCB::running->state = PCB::BLOCKED;
		myPCB->waitQueue.add(PCB::running, 0);
		unlockCout
		dispatch();
	}
}

ID Thread::getId() { return myPCB ? myPCB->id : 0; }

Thread* Thread::getThreadById(ID id) {
	return PCB::ready.PCBById(id)->myThread;
}

void Thread::signal(SignalId signal) {
	if(myPCB) myPCB->signal(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	if(myPCB) myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id) {
	if(myPCB) myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	if(myPCB) myPCB->swap(id, hand1, hand2);
}

void Thread::blockSignal(SignalId signal) {
	if(myPCB) myPCB->blockSignal(signal);
}

void Thread::blockSignalGlobally(SignalId signal) {
	PCB::blockSignalGlobally(signal);
}

void Thread::unblockSignal(SignalId signal) {
	if(myPCB) myPCB->unblockSignal(signal);
}

void Thread::unblockSignalGlobally(SignalId signal) {
	PCB::unblockSignalGlobally(signal);
}

void dispatch() {
	PCB::dispatch();
}
