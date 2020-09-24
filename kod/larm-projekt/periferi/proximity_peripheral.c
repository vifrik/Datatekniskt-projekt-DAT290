#include "proximity_sensor.h"
#include "stk.h"
#include "can.h"

void proximity_peripheral_init(void) {
	//state_init();
	stk_init();
	proximity_init();
}

void proximity_peripheral_think(void) {	
	while(1) {
		uchar proximity_distance = proximity_read();
		DUMP_numeric(proximity_distance);
		
		// Delay 10^6 us = 1s
		delay(1000000);
	}
}