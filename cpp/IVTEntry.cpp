#include "IVTEntry.h"
#include <dos.h>
#include "pcb.h"
#include "kernlev.h"

IVTEntry* IVTEntry::entries[256] = { 0 };

IVTEntry* IVTEntry::getEntry(IVTNo ivtNo) {
	return entries[ivtNo];
}

IVTEntry::IVTEntry(IVTNo ivtNo, void interrupt (*newIRoutine)()) {
	lock
	this->ivtNo = ivtNo;
	entries[ivtNo] = this;
	oldIRoutine = getvect(ivtNo);
	setvect(ivtNo, (pInterrupt)newIRoutine);
	myEvent = 0;
	unlock
}

void IVTEntry::callOld() {
	(*oldIRoutine)();
}

void IVTEntry::signal() {
	if(myEvent) myEvent->signal();
}

IVTEntry::~IVTEntry() {
	lock
	setvect(ivtNo, oldIRoutine);
	entries[ivtNo] = 0;
	unlock
}
