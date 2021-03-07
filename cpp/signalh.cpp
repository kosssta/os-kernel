/*#include "signalh.h"
#include <iostream.h>
#include "pcb.h"

int Signal::blockedGlobally[16] = { 0 };

Signal::Signal() {
	for (unsigned i = 0; i < 16; i++) {
		signals[i] = 0;
		blocked[i] = 0;
	}
	firstSign = lastSign = 0;
}

Signal::Signal(const Signal &s) {
	lockCout
	for (unsigned i = 0; i < 16; i++) {
		signals[i] = 0;
		if (!s.signals[i]) continue;
		SignalHandlerNode *node = s.signals[i]->next, *start = node;
		append(i, node->handler);
		node = node->next;
		while (start != node) {
			append(i, node->handler);
			node = node->next;
		}
	}
	unlockCout
}

void Signal::append(unsigned i, SignalHandler handler) {
	if (i >= 0 && i <= 15) {
		lock
		SignalHandlerNode *newNode = new SignalHandlerNode(handler);
		unlock;
		lockCout
		if (signals[i]) {
			newNode->next = signals[i]->next;
			signals[i]->next = newNode;
			signals[i] = signals[i]->next;
		}
		else {
			signals[i] = newNode;
			newNode->next = newNode;
		}
		unlockCout
	}
}

Signal::~Signal() {
	for (unsigned i = 0; i < 16; i++) {
		removeAll(i);
	}
}
/*
void Signal::remove(unsigned i, SignalHandler handler) {
	if (i >= 0 && i <= 15 && signals[i]) {
		SignalHandlerNode *prev = signals[i], *tmp = signals[i]->next, *begin = prev;
		if (prev == tmp && prev->handler == handler) {
			delete tmp;
			signals[i] = nullptr;
			return;
		}
		if (tmp->handler == handler) {
			prev->next = tmp->next;
			delete tmp;
			return;
		}
		prev = tmp; tmp = tmp->next;
		while (prev != begin && tmp->handler != handler) {
			prev = tmp; tmp = tmp->next;
		}
		if (prev == begin) return;
		if (tmp == begin) signals[i] = prev;
		prev->next = tmp->next;
		delete tmp;
	}
}

void Signal::swap(unsigned i, SignalHandler h1, SignalHandler h2) {
	lockCout
	if (i >= 0 && i <= 15 && signals[i] && h1 != h2) {
		SignalHandlerNode *tmp = signals[i], *begin = tmp, *hn1 = 0, *hn2 = 0;
		if (tmp->handler == h1) hn1 = tmp;
		else if (tmp->handler == h2) hn2 = tmp;
		tmp = tmp->next;
		while (tmp != begin && hn1 == 0 && hn2 == 0) {
			if (tmp->handler == h1) hn1 = tmp;
			else if (tmp->handler == h2) hn2 = tmp;
			tmp = tmp->next;
		}
		if (tmp == begin) {
			unlockCout
			return;
		}
		while (tmp != begin && hn1 == 0) {
			if (tmp->handler == h1) hn1 = tmp;
			tmp = tmp->next;
		}
		while (tmp != begin && hn2 == 0) {
			if (tmp->handler == h2) hn2 = tmp;
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

void Signal::signal(unsigned i) {
	lockCout
	lastSign = (firstSign ? lastSign->next : firstSign) = new SignalNode(i);
	unlockCout
}

void Signal::obradi() {
	asm {
		pushf
		sti
	}
	lockCout
	SignalHandlerNode *node, *end; int i = 0;
	SignalNode *old, *prev = 0, *tmp = firstSign;
	while (tmp) {
		node = signals[tmp->i];
		if (!blockedGlobally[tmp->i] && !blocked[tmp->i]) {
			if (node) {
				end = node = node->next;
				cout << tmp->i << " - " << node->handler << " - " << ++i << endl;
				node = node->next;
				while (node != end) {
					cout << tmp->i << " - " << node->handler << endl;
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
	asm popf
	unlockCout
}

void Signal::removeAll(unsigned i) {
	lockCout
	if (signals[i]) {
		SignalHandlerNode *node = signals[i]->next, *old;
		signals[i]->next = 0;
		while (node) {
			old = node;
			node = node->next;
			lock
			delete old;
			unlock
		}
		signals[i] = 0;
	}
	unlockCout
}

void Signal::block(unsigned i) {
	lockCout
	blocked[i] = 1;
	unlockCout
}

void Signal::blockGlobally(unsigned i) {
	lockCout
	blockedGlobally[i] = 1;
	unlockCout
}

void Signal::unblock(unsigned i) {
	lockCout
	blocked[i] = 0;
	unlockCout
}

void Signal::unblockGlobally(unsigned i) {
	lockCout
	blockedGlobally[i] = 0;
	unlockCout
}
*/
