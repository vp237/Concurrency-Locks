#include "cond.h"
#include "lock.h"

void c_init(lock_t* l, cond_t* c) {
	c->waiting_queue = NULL;
}	

void c_wait(lock_t* l, cond_t* c) {
	PIT->CHANNEL[0].TCTRL = 1; // disabling interrupt
	addProcess(current_process, (&c->waiting_queue));
	l_unlock(l);
	current_process->blocked = BLOCKED;
	process_blocked();
	l_lock(l);
	PIT->CHANNEL[0].TCTRL = 3; // reenabling interrupt
}	

int c_waiting(lock_t* l, cond_t* c) {
	if (c->waiting_queue == NULL) {
		return NOT_WAITING;
	} 
	else {
		return WAITING;
	}	
}	

void c_signal(lock_t* l, cond_t* c) {
	PIT->CHANNEL[0].TCTRL = 1; // disabling interrupt
	if (c->waiting_queue != NULL) {
		process_t * temp = removeProcess(&c->waiting_queue);
		addProcess(temp, (&process_queue));
		temp->blocked = NOT_BLOCKED;
	}	
	l_unlock(l);
	PIT->CHANNEL[0].TCTRL = 3; // reenabling interrupt
}	
