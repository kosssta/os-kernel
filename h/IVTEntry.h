#ifndef _IVTEntry_h_
#define _IVTEntry_h_

class KernelEv;
typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

class IVTEntry {
public:
	static IVTEntry* getEntry(IVTNo ivtNo);
	IVTEntry(IVTNo ivtNo, void interrupt (*newIRoutine)());
	void callOld();
	void signal();
	~IVTEntry();
	static IVTEntry* entries[256];
private:
	friend class KernelEv;
	pInterrupt oldIRoutine;
	KernelEv* myEvent;
	IVTNo ivtNo;
};

#define PREPAREENTRY(a, b) \
	void interrupt newIRoutine##a() {\
	if(IVTEntry::entries[a]) {\
		IVTEntry::entries[a]->signal();\
		cout << "% " << a << " ";\
		if(b == 1) { IVTEntry::entries[a]->callOld(); }\
	}\
}\
	IVTEntry entry##a((IVTNo)a, newIRoutine##a);


#endif
