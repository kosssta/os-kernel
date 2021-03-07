#ifndef _SemList_h_
#define _SemList_h_

class KernelSem;

struct SemListNode {
	SemListNode *next;
	KernelSem *sem;
	SemListNode(KernelSem *s, SemListNode *n = 0) : sem(s), next(n) { }
};

class SemList {
public:
	SemList();
	SemList(KernelSem *s);

	void append(KernelSem *s);
	void remove(KernelSem *s);
	void timeRoutine();
	virtual ~SemList();
private:
	SemListNode *head, *tail;
};

#endif
