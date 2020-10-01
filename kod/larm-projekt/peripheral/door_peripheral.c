#include "stk.h"
#include "can.h"
#include "messages.h"
#include "door_sensor.h"
#include "shared_peripheral.h"

extern State state;

typedef struct {
	unsigned char active;
	unsigned char tolerance;
} DoorState;

DoorState door_states[7];
unsigned char number_of_doors;

void ndoors_handler(CANMsg *msg) {
	number_of_doors = msg->buff[0];
}

void update_tolerance(CANMsg *msg) {
	unsigned char door_id = msg->buff[0];
	door_states[door_id].tolerance = msg->buff[1];
}

void activate_on_handler(CANMsg *msg) {
	if (msg->length) {
		unsigned char door_id = msg->buff[0];
		door_states[door_id].active = 1;
		green_lamp_disable(door_id);
	} else {
		state.active = 1;
	}
}

void activate_off_handler(CANMsg *msg) {
	if (msg->length) {
		unsigned char door_id = msg->buff[0];
		door_states[door_id].active = 1;
		green_lamp_enable(door_id);
	} else {
		state.active = 0;
	}
}

// Hanterar CAN-meddelanden
void door_receiver(void) {

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
				activate_on_handler(&msg);
				break;
			case ACTIVE_OFF:
				activate_off_handler(&msg);
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
	request_id(DOOR, number_of_doors);
}

// Huvudslinga för periferienhet
void door_peripheral_think(void) {	
	while(1) {
		char door_status = door_read(0);
		//DUMP_numeric(door_status);
		
		if (!state.alarm && door_status) {
			alarm_raise();
		}
		
		//DUMP_numeric(proximity_read());
		
		// Delay 10^6 us = 1s
		delay(1000000);
	}
}