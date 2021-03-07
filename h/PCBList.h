#ifndef _PCBlist_h_
#define _PCBlist_h_

class PCB;
typedef unsigned Time;

struct PCBListNode {
	PCBListNode *next;
	PCB *pcb;
	Time &blockTime;
	int zero;
	PCBListNode(PCB *p, Time &blckTime, PCBListNode *n = 0) :
		pcb(p), blockTime(blckTime), next(n) { zero = blckTime == 0; }
};

class PCBList {
public:
	PCBList(PCB *pcb, Time &blckTime);
	PCBList();

	void add(PCB *pcb, Time &blockTime);
	//void put(PCB *pcb, Time &blockTime);
	PCB* PCBById(unsigned long id);
	PCB* get();
	int isEmpty();
	virtual ~PCBList();
	int unblock();
	void removeAll();
//private:
	friend class KernelSem;
	PCBListNode *head, *tail;
};

#endif
