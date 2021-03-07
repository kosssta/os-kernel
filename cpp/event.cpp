#include "event.h"
#include "kernlev.h"
#include "pcb.h"

Event::Event(IVTNo ivtNo) {
	lock
	myImpl = new KernelEv(ivtNo);
	unlock
}

Event::~Event() {
	lock
	delete myImpl;
	unlock
}

void Event::signal() {
	myImpl->signal();
}

void Event::wait() {
	myImpl->wait();
}
