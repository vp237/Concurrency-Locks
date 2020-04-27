#include "3140_concur.h"

#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

#define BLOCKED 1 /* the process is blocked */
#define NOT_BLOCKED 0 /* the process is not blocked */

#define LOCK 1 /* the lock is being used */
#define UNLOCK 0 /* the lock is free to acquire */

#define WAITING 1 /* there are processes in the waiting queue */
#define NOT_WAITING 0 /* there are no processes in the waiting queue */

/** Implement your structs here */

/**
 * This structure holds the process structure information
 */
struct process_state {
	unsigned int stack_size;  /* the size of the stack */   
	unsigned int * sp;  /* the stack pointer for the process */ 
	unsigned int * original_sp; /* the original stack pointer for the process */  
	unsigned int blocked; /* the blocked state of the process */
	struct process_state * next;   /* the next process */
};

/**
 * This defines the lock structure
 */
typedef struct lock_state {
	unsigned int in_use; /* whether the lock is in use or not */
	process_t * blocked_queue; /* the blocked queue associated with the lock */
} lock_t;

/**
 * This defines the conditional variable structure
 */
typedef struct cond_var {
	process_t * waiting_queue; /* the waiting queue for the conditional variable */
} cond_t;

/**
 * These are the helper functions (see process.c for implementation and specification)
 */

void addProcess (process_t * next_process, process_t * (*queue));

process_t * removeProcess (process_t * (*queue));

#endif
