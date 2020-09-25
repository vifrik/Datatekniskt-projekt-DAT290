#include "stk.h"
#include "can.h"
#include "messages.h"
#include "door_sensor.h"
#include "shared_peripheral.h"

extern State state;

// Hanterar CAN-meddelanden
void door_receiver(void) {
	DUMP("CAN message received: ");

	CANMsg msg;
	can_receive(&msg);

	if (msg.nodeId == state.id) {
		switch(msg.msgId) {
			case ALARM:
				break;
			case ALARM_OFF:
				alarm_lower();
				break;
			case POLL_REQUEST:
				poll_respond(&msg);
				break;
			case POLL_RESPONSE:
				break;
			case DICP_REQUEST:
				break;
			case DICP_RESPONSE:
				update_id(&msg);
				break;
			case TOL_SET:
				update_tolerance(&msg);
				break;
			case ACTIVE_ON:
				state.active = 1;
				break;
			case ACTIVE_OFF:
				state.active = 0;
				break;
		}
	}
}

// Initialisering av dörrenhet
void door_peripheral_init(void) {
	state_init();
	stk_init();
	door_init();
	can1_init(door_receiver);
	request_id();
}

// Huvudslinga för periferienhet
void door_peripheral_think(void) {	
	while(1) {
		DUMP("loop");
		uchar door_status = door_read();
		DUMP_numeric(door_status);
		
		if (!state.alarm && door_status) {
			alarm_raise();
		}
		
		//DUMP_numeric(proximity_read());
		
		// Delay 10^6 us = 1s
		//delay(1000000);
	}
}