#include "stk.h"
#include "proximity.h"
#include "door.h"

void think() {
	stk_init();
	
	door_init();
	proximity_init();
	
	while(1) {
		//DUMP_numeric(door_read());
		DUMP_numeric(proximity_read());
		
		// Delay 10^6 us = 1s
		delay(1000000);
	}
}