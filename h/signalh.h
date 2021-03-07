#ifndef _signalh_h_
#define _signalh_h_
/*
class Thread;

typedef void (*SignalHandler)();

typedef unsigned SignalId;
/*
struct SignalHandlerNode {
	SignalHandler handler;
	SignalHandlerNode *next;
	SignalHandlerNode(SignalHandler s, SignalHandlerNode* n = 0) : handler(s), next(n) {}
};

struct SignalNode {
	SignalId i;
	SignalNode *next;
	SignalNode(SignalId in, SignalNode *n = 0) : i(in), next(n) { }
};

class Signal {
public:
	SignalHandlerNode* signals[16];
	SignalNode* firstSign, *lastSign;
	static int blockedGlobally[16];
	int blocked[16];

	friend class Thread;
	Signal();
	Signal(const Signal &s);
	~Signal();

	void append(unsigned i, SignalHandler handler);
	//void remove(unsigned i, SignalHandler handler);
	void removeAll(unsigned i);
	void swap(unsigned i, SignalHandler h1, SignalHandler h2);
	void signal(unsigned i);
	void obradi();
	void block(unsigned i);
	static void blockGlobally(unsigned i);
	void unblock(unsigned i);
	static void unblockGlobally(unsigned i);
};
*/
#endif
