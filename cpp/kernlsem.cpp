#include "kernlsem.h"
#include "pcb.h"
#include <iostream.h>
SemList KernelSem::semList;

KernelSem::KernelSem(int init) : value(init) {
	semList.append(this);
}

KernelSem::~KernelSem() {
	semList.remove(this);
}

int KernelSem::wait(Time maxTimeToWait) {
	lockCout
	if(--value < 0) {
		PCB::running->state = PCB::BLOCKED;
		blocked.add(PCB::running, maxTimeToWait);
		unlockCout
		dispatch();
		return maxTimeToWait;
	}
	unlockCout
	return 1;
}

int KernelSem::signal(int n) {
	lockCout
	if(n > 0) {
		int i = 0;
		while(i < n && !blocked.isEmpty()) {
			PCB* bl = blocked.get();
			bl->state = PCB::READY;
			Scheduler::put(bl);
			i++;
		}
		value += n;
		return i;
	} else if(n == 0 && value++ < 0) {
		PCB* bl = blocked.get();
		if (bl) {
			bl->state = PCB::READY;
			Scheduler::put(bl);
		}
	}
	unlockCout
	return n;
}
