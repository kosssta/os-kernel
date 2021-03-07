#include "PCBList.h"
#include "pcb.h"
#include <iostream.h>

PCBList::PCBList() : head(0), tail(0) { }

PCBList::PCBList(PCB *pcb, Time &blckTime) {
	lock
	head = tail = new PCBListNode(pcb, blckTime);
	unlock
}

void PCBList::add(PCB *pcb, Time &blockTime) {
	if(blockTime == 0) {
		lock
		tail = (head ? tail->next : head) = new PCBListNode(pcb, blockTime);
		unlock
	}
	else {
		PCBListNode *prev = 0, *tmp = head;
		while(tmp && (int)(blockTime - tmp->blockTime) > 0) {
			if(tmp->zero) break;
			blockTime -= tmp->blockTime;
			prev = tmp;
			tmp = tmp->next;
		}
		if(tmp) {
			PCBListNode *newNode;
			if(prev) {
				lock
				newNode = new PCBListNode(pcb, blockTime, tmp);
				unlock
				prev->next = newNode;
			}
			else {
				lock
				newNode = new PCBListNode(pcb, blockTime, head);
				unlock
				if(!head) head = tail = newNode;
				else head = newNode;
			}
			if(!tmp->zero) tmp->blockTime -= newNode->blockTime;
		} else tail = (prev ? prev->next : head) = new PCBListNode(pcb, blockTime);
	}
}
/*
void PCBList::put(PCB *pcb, Time blockTime) {
	asm cli
	PCBListNode *node = new PCBListNode(pcb, blockTime, head);
	if(!head) tail = node;
	head = node;
	asm sti
}
*/
int PCBList::isEmpty() { return !head; }

PCB* PCBList::get() {
	lockCout
	if(!head) return 0;
	PCB* ret = head->pcb;
	PCBListNode *old = head;
	head = head->next;
	if(!head) tail = 0;
	old->blockTime = 1;
	lock
	delete old;
	unlock
	unlockCout
	return ret;
}

PCB* PCBList::PCBById(unsigned long id) {
	PCBListNode *tmp = head;
	while(tmp) {
		if(tmp->pcb->id == id) return tmp->pcb;
		tmp = tmp->next;
	}
	return 0;
}

PCBList::~PCBList() {
	removeAll();
}

void PCBList::removeAll() {
	PCBListNode *old;
	while(head) {
		old = head;
		head = head->next;
		old->pcb->state = PCB::READY;
		Scheduler::put(old->pcb);
		delete old;
	}
}

int PCBList::unblock() {
	int i = 0;
	if(head && !head->zero) {
		head->blockTime--;
	}
	while(head && !head->zero && !head->blockTime) {
		//cout << "A" << endl;
		PCBListNode *old = head;
		head = head->next;
		old->pcb->state = PCB::READY;
		Scheduler::put(old->pcb);
		delete old;
		i++;
	}
	if(!head) tail = 0;
	return i;
}
