#include "SemList.h"
#include "kernlsem.h"
#include "pcb.h"
SemList::SemList() : head(0), tail(0) { }

SemList::SemList(KernelSem *s) {
	lock
	tail = head = new SemListNode(s);
	unlock
}

void SemList::append(KernelSem *s) {
	lock
	tail = (head ? tail->next : head) = new SemListNode(s);
	unlock
}

void SemList::remove(KernelSem *s) {
	lock
	if(!head) return;
	SemListNode *prev, *tmp;
	prev = 0; tmp = head;
	while(tmp) {
		if(tmp->sem == s) break;
		prev = tmp;
		tmp = tmp->next;
	}
	if(!tmp) return;

	if(prev) prev->next = tmp->next;
	else {
		head = head->next;
		if(!head) tail = 0;
	}
	delete tmp;
	unlock
}

void SemList::timeRoutine() {
	SemListNode *s;
	int i;
	s = head;
	while(s) {
		i = s->sem->blocked.unblock();
		s->sem->value += i;
		s = s->next;
	}
}

SemList::~SemList() {
	lock
	SemListNode *old;
	while(head) {
		old = head;
		head = head->next;
		delete old;
	}
	unlock
}
