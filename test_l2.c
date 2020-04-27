#include "3140_concur.h"
#include "utils.h"
#include "lock.h"

//Test case 2: This test case involves a combination of two processes that uses a shared lock l (process 2 and 3) and a process that does not use any locks (process 1).
//Process 2 grabs the lock first, then process 3 unlocks it and grabs the lock. Finally the lock is released.
//Expected behavior: The red LED blinks 1 time first, then the blue and green LEDs light up almost simultaneously, then the red LED blinks 4 more times (during this time
//the blue and green LED are still on). Finally, only the green LED light up.

lock_t l;

void p1 (){
	int i;
	for (i=0; i < 10; i++) {
		delay();
		LEDRed_Toggle();
	}
}

void p2 () {
	delay();
	l_lock(&l);
	delay();
	LEDBlue_Toggle();
	delay();
	l_unlock(&l);
	delay();
}	

void p3 () {
	delay();
	l_unlock(&l);
	delay();
	LEDGreen_Toggle();
	delay();
	l_lock(&l);
	delay();
	l_unlock(&l);
}	

int main (void){		
	LED_Initialize();           /* Initialize the LEDs           */	
	l_init (&l);
 
	if (process_create (p1, 20) < 0) {
	 	return -1;
	}
	if (process_create (p2, 20) < 0) {
	 	return -1;
	}
	if (process_create (p3, 20) < 0) {
	 	return -1;
	}
	
	process_start();
 
	delay();
	LEDGreen_On();

	while(1);
	return (0);
}	