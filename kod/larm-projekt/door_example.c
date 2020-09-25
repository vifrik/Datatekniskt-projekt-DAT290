#include "messages.h"
#include "can.h"

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

void arr_clear(uchar *arr[]){
	uchar length = sizeof(*arr)/sizeof(*arr[0]);
	for(int i = 0;i<length;i++){
		arr[i] = 0;
	}
}

void alarm_raise() {
    state.alarm = 1;

    CANMsg msg;

    msg.msgId = ALARM;
    msg.nodeId = state.id;
    msg.length = 0;
    arr_clear(&(msg.buff));

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
    arr_clear(&(response.buff));
    response.buff = ~msg->buff;

    can_send(&response);
}

void request_id() {
    CANMsg msg;
    msg.msgId = DICP_REQUEST;
    msg.nodeId = state.id;
    msg.length = 0;
    arr_clear(&(msg.buff));

    can_send(&msg);
}

void update_door_id(CANMsg *msg) {
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

void init(void) {
    state_init();
    can_init1(receiver);

    request_id();
}

void think() {
    // Read door sensor here
    // If 1 (open), alarm_raise();
}