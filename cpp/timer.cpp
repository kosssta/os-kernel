#include "timer.h"
//void tick();

int Timer::cnt = defaultTimeSlice;
int Timer::dmnd = 0;
unsigned oldTimerSeg, oldTimerOff;
unsigned newTimerSeg = FP_SEG(&Timer::myRoutine);
unsigned newTimerOff = FP_OFF(&Timer::myRoutine);

void Timer::dispatch() {
	lock
	dmnd = 1;
	myRoutine();
	unlock
}

void interrupt Timer::myRoutine() {
	static unsigned tss, tsp, tbp;

	if(!dmnd) {
		asm int 60h;
		KernelSem::semList.timeRoutine();
	}
	if(PCB::running->timeSlice != 0 || dmnd) {
		if(!dmnd) cnt--;
		if(cnt == 0 || dmnd) {
			if (PCB::lockFlag) {
				dmnd = 0;
				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}

				PCB::running->sp = tsp;
				PCB::running->ss = tss;
				PCB::running->bp = tbp;

				if(PCB::running->state != PCB::FINISHED && PCB::running->state != PCB::BLOCKED) {
					Scheduler::put(PCB::running);
				}
				PCB::running = Scheduler::get();
				if(!PCB::running) PCB::running = PCB::idle->myPCB;

				tsp = PCB::running->sp;
				tss = PCB::running->ss;
				tbp = PCB::running->bp;
				asm {
					mov sp, tsp
					mov ss, tss
					mov bp, tbp
				}

				cnt = PCB::running->timeSlice;

				PCB::running->obradi();
			}
			else dmnd = 1;
		}
	}
}

void Timer::inic() {
	lock
	asm {
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSeg, ax
		mov word ptr es:0182h, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOff, ax
		mov word ptr es:0180h, ax

		mov ax, word ptr newTimerSeg
		mov word ptr es:0022h, ax
		mov ax, word ptr newTimerOff
		mov word ptr es:0020h, ax

		pop ax
		pop es
	}
	unlock
}

void Timer::restore() {
	lock
	asm {
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr oldTimerSeg
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOff
		mov word ptr es:0020h, ax

		pop ax
		pop es
	}
	unlock
}
