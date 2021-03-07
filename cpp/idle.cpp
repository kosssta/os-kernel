#include "idle.h"
#include <iostream.h>
#include "pcb.h"

void Idle::run() {
	unsigned long i = 0;
	while(!acq) {
		//if(PCB::main->state == PCB::BLOCKED && PCB::main->myThread->myPCB->state == PCB::FINISHED)
			//Scheduler::put(PCB::main);
		//for(; i >= 0; i++)
		//cout << "IDLE" << i++ << endl;
		//dispatch();
	}
}
