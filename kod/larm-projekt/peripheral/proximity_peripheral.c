#include "stk.h"
#include "can.h"
#include "messages.h"
#include "proximity_sensor.h"
#include "vibration_sensor.h"
#include "shared_peripheral.h"
#include "lamp.h"
#include "debug.h"

extern State state;
uchar tolerance = 10;
uchar distance_active = 1;
uchar vibration_active = 1;
unsigned int dist = 0xFFFF;

void update_tolerance_proximity(CANMsg *msg) {
	tolerance = msg->buff[1];
}

void proximity_set_active(CANMsg *msg) {
	print_can_msg(*msg);
	msg->buff[1] ? usart_send("Enabled ") : usart_send("Disabled ");
	msg->buff[0] ? usart_sendl("vibration sensor") : usart_sendl("distance sensor");
	
	if (msg->buff[0]) {
		vibration_active = msg->buff[1];
	} else {
		distance_active = msg->buff[1];
	}
}

// Hanterar CAN-meddelanden
void proximity_receiver(void) {
	CANMsg msg;
	can_receive(&msg);
	

	// Om meddelandets mottagare matchar mitt id
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
				update_tolerance_proximity(&msg);
				break;
			case ACTIVATE:
				proximity_set_active(&msg);
		}
	}
}

void proximity_callback(unsigned int distance) {
	dist = distance;
}

void vibration_callback(void) {
	if (!state.alarm && state.id != 0xF && vibration_active) {
		alarm_raise(1);
	}
}

// Initialisering av rörelsesensor och CAN
void proximity_peripheral_init(void) {
	state_init();
	lamp_init();

	proximity_init();
	proximity_callback_init(proximity_callback);

	vibration_init();
	vibration_callback_init(vibration_callback);
	
	stk_init();
	
	can1_init(proximity_receiver);
	request_id(PROXIMITY, 2);
}

// Huvudslinga för periferienhet
void proximity_peripheral_think(void) {
	while(state.id == 0xF);

	while(1) {	
		// Om vi inte har alarm och avståndssensorn är aktiv
		if (!state.alarm && distance_active) {
			// Om distans är mincre än tolerans
			if (dist < tolerance)
				alarm_raise(0);
			else if (dist < tolerance*2)
				red_lamp_enable();
			else 
				red_lamp_disable();
		}
		
		proximity_read();
		delay(25000);
	}
}