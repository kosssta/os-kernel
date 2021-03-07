#include "signalh.h"
#include <iostream.h>
#include "pcb.h"
/*#include "IVTEntry.h"
#include "event.h"
#include "thread.h"

PREPAREENTRY(9, 1)

Event ev(9);

class A : public Thread {
protected:
	void run();
public:
	~A() { waitToComplete(); }
};

void A::run() {
	for(int i = 0; i < 50; i++) {
		ev.wait();
		ev.wait();
		lockCout
		cout << "===========\n";
		unlockCout
	}
}

int userMain(int argc, char *argv[]) {
	Signal *s = new Signal; A *a = new A;
	a->start();
	s->append(1, 5); s->append(1, 6);
	for(int i = 0; i < 3000; i++) {
		s->signal(1);
		s->signal(1);
		s->signal(0);
		s->signal(28);
	}
	s->obradi();
	delete s;
	delete a;
	A* a2 = new A;
	a2->start();
	delete a2;
	return 0;
}
*/
