#include "3140_concur.h"
#include "utils.h"
#include "lock.h"

//Test case 1: This test case involves a deadlock. Process 1 grabs lock l1 and process 2 grabs lock l2. Then when process 1 tries to grab lock l2 it is blocked,
//and when process 2 tries to grab lock l1 it is also blocked. Then both processes will be suspended, or in other words, they are in a deadlock.
//Expected behavior: The red LED lights up then the blue LED lights up along with the red LED. Finally, the green LED lights up.

lock_t l1;
lock_t l2;

void p1 () {
	int i;
	for (i = 0; i <5; i++) {
		l_lock(&l1);
		delay();
		LEDRed_Toggle();
		delay();
		l_lock(&l2);
		delay();
		LEDRed_Toggle();
		l_unlock(&l2);
		delay();
		l_unlock(&l1);
	}	
}

void p2 () {
	int i;
	delay();
	for (i = 0; i <5; i++) {
		l_lock(&l2);
		delay();
		LEDBlue_Toggle();
		l_lock(&l1);
		delay();
		LEDBlue_Toggle();
		l_unlock(&l1);
		l_unlock(&l2);
	}	
}

int main (void){		
	LED_Initialize();           /* Initialize the LEDs           */	
	l_init (&l1);
	l_init (&l2);
 
	if (process_create (p1, 20) < 0) {
	 	return -1;
	}
	if (process_create (p2, 20) < 0) {
	 	return -1;
	}
	
	process_start();
 
	delay();
	LEDGreen_On();

	while(1);
	return (0);
}	