#include "lock.h"

void l_init (lock_t *l) {
	l->blocked_queue = NULL; 
	l->in_use = UNLOCK;
}

void l_lock (lock_t *l) {
	PIT->CHANNEL[0].TCTRL = 1; // disabling interrupt
	if (l->in_use == UNLOCK) { // if the lock is not in use
		l->in_use = LOCK; 
	}	
	else { // if the lock is in use
		current_process->blocked = BLOCKED;
		addProcess(current_process, (&l->blocked_queue));
	}	
	process_blocked();
	PIT->CHANNEL[0].TCTRL = 3; // reenabling interrupt
}	

void l_unlock (lock_t *l) {
	PIT->CHANNEL[0].TCTRL = 1; // disabling interrupt
	if (l->blocked_queue != NULL) {
		process_t * ready = removeProcess(&l->blocked_queue);
		ready->blocked = NOT_BLOCKED;
		addProcess(ready, (&process_queue));
	}
	else {
		l->in_use = UNLOCK;
	}	
	PIT->CHANNEL[0].TCTRL = 3; // reenabling interrupt
}	

