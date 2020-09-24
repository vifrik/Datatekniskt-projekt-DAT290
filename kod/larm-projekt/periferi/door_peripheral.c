#include "stk.h"
#include "can.h"
#include "messages.h"
#include "door_sensor.h"

typedef unsigned char uchar;

typedef struct {
    uchar active;
    uchar id;
    uchar alarm;
    uchar tolerance;
} State;

State state;

void state_init(void) {
    state.active = 0;
    state.id = 0;
    state.alarm = 0;
    state.tolerance = 0;
}

void alarm_raise() {
    state.alarm = 1;

    CANMsg msg;

    msg.msgId = ALARM;
    msg.nodeId = state.id;
    msg.length = 0;
	for (int i = 0; i < 8; i++) {
		msg.buff[i] = 0;
	}

    can_send(&msg);
}

void alarm_lower() {
    state.alarm = 0;
}

void poll_respond(CANMsg *msg) {
    CANMsg response;
    response.msgId = POLL_RESPONSE;
    response.nodeId = state.id;
    response.length = 2;
	for (int i = 0; i < 8; i++) {
		response.buff[i] = ~msg->buff[i];
	}
    

    can_send(&response);
}

void request_id() {
	DUMP("Requesting ID");
	
    CANMsg msg;
    msg.msgId = DICP_REQUEST;
    msg.nodeId = state.id;
    msg.length = 0;
	for (int i = 0; i < 8; i++) {
		msg.buff[i] = 0;
	}

    can_send(&msg);
}

void update_door_id(CANMsg *msg) {
	DUMP("Recieved ID");
	DUMP_numeric(msg->buff[0]);
    state.id = msg->buff[0];
}

void update_tolerance(CANMsg *msg) {
    state.tolerance = msg->buff[0];
}

void receiver(void) {
	DUMP("CAN message received: ");
	
    CANMsg msg;
    can_receive(&msg);
	
    if (!state.id || msg.msgId == state.id) {
        switch(msg.msgId) {
            case ALARM:
                break;
            case ALARM_OFF:
                alarm_lower();
                break;
            case POLL_REQUEST:
                break;
            case POLL_RESPONSE:
                poll_respond(&msg);
                break;
            case DICP_REQUEST:
                break;
            case DICP_RESPONSE:
                update_door_id(&msg);
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

void peripheral_init(void) {
	state_init();
	stk_init();
	door_init();
	can1_init(receiver);
    request_id();
}

void peripheral_think(void) {	
	while(1) {
		uchar door_status = door_read();
		DUMP_numeric(door_status);
		
		if (door_status) {
			alarm_raise();
		}
		
		//DUMP_numeric(proximity_read());
		
		// Delay 10^6 us = 1s
		delay(1000000);
	}
}