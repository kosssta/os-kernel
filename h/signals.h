#ifndef _signals_h_
#define _signals_h_

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

#endif
