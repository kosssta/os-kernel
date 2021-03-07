#include "kernlev.h"
#include "IVTEntry.h"
#include "pcb.h"

KernelEv::KernelEv(IVTNo ivtNo) : value(0) {
	if(IVTEntry::entries[ivtNo])
	IVTEntry::entries[ivtNo]->myEvent = this;
	owner = PCB::running;
	this->ivtNo = ivtNo;
}

void KernelEv::signal() {
	lock
		if(owner->state == PCB::BLOCKED) {
			owner->state = PCB::READY;
			Scheduler::put(owner);
		} else value = 1;
	unlock
	//dispatch();
}

void KernelEv::wait() {
	lockCout
	if (owner == PCB::running) {
		if(value) {
			value = 0;
		}
		else {
			owner->state = PCB::BLOCKED;
			unlockCout
			dispatch();
			return;
		}
	}
	unlockCout
}

KernelEv::~KernelEv() {
	lockCout
	if(owner->state == PCB::BLOCKED) {
		owner->state = PCB::READY;
		Scheduler::put(owner);
	}
	IVTEntry::entries[ivtNo]->myEvent = 0;
	unlockCout
}
