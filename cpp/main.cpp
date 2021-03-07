#include "pcb.h"
#include "timer.h"
#include <iostream.h>

extern int userMain(int argc, char *argv[]);

void main(int argc, char *argv[]) {
	PCB::createMainPCB();
	Timer::inic();

	int ret = userMain(argc, argv);
	cout << "\nUserMain returned " << ret << endl;

	Timer::restore();
	PCB::deleteMainPCB();
}
