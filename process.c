#include "3140_concur.h"
#include "shared_structs.h"
#include <stdlib.h>
#include <MK64F12.h>

process_t * current_process = NULL; //the currently running process

process_t * process_queue = NULL; //the first process in the queue of scheduled processes

int process_create(void (*f)(void), int n) {
	process_t * state = malloc(sizeof(process_t)); //allocates memory for process
	if (state == NULL) {
		return -1;
	}
	unsigned int * stateOfProcess = process_stack_init(f, n); //state of process
	if (stateOfProcess == NULL) {
		free(state);
		return -1;
	}
	else {
		state->sp = stateOfProcess;
		state->original_sp = stateOfProcess;
		state->next = NULL;
		state->stack_size = n;
		state->blocked = NOT_BLOCKED;
		addProcess(state, (&process_queue));
		return NULL;
	}
}	

void process_start (void) {
	//Setting up the timer PIT0
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT_MCR = 0;
	PIT_LDVAL0 = SystemCoreClock/100;
	NVIC_EnableIRQ(PIT0_IRQn);
	process_begin();
}	

unsigned int * process_select (unsigned int * cursp) {
	if (cursp == NULL) { 
		if (current_process != NULL) { 
			process_stack_free(current_process->original_sp, current_process->stack_size);
			free(current_process);
		}
		if (process_queue == NULL) {
			return NULL;
		}
		else {
			current_process = removeProcess((&process_queue));
			return current_process->sp;
		}
	}
	else { 
		if (current_process->blocked == NOT_BLOCKED) {
			addProcess(current_process, (&process_queue));
		}
		current_process->sp = cursp;
		current_process = removeProcess((&process_queue));
		if (current_process == NULL) {
			return NULL;
		}
		else {
			return current_process->sp;
		}
	}
}	
		
//helper functions

//adds process to the end of process_queue

void addProcess (process_t * next_process, process_t * (*queue)) {
	if ((*queue) == NULL) {
		(*queue) = next_process;
		next_process->next = NULL;
	}
	else {
		process_t * tmp = (*queue);
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = next_process;
		next_process->next = NULL;
	}	
}	

//removes the first process in process_queue and return the first process of the queue after the removal

process_t * removeProcess (process_t * (*queue)) {
	if ((*queue) == NULL) {
		return NULL;
	}
	else {
		process_t * temp = (*queue);
		(*queue) = (*queue)->next;
		temp->next = NULL;
		return temp;
  }
}			



