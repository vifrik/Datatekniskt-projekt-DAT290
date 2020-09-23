#include "messages.h"

unsigned char id;

void setDoorId(uchar data) {
    id = msg.data[0];
}

void alarm_raise() {
    CANMsg msg;

    msg.msgId = ALARM;
    msg.nodeId = id;
    msg.length = 0;
    msg.buff = 0;

    can_send(&msg);
}

void receiver(void) {
	DUMP("CAN message received: ");
	
    CANMsg msg;
    can_receive(&msg)
	
    switch(msg.msgId) {
        case ALARM:
            break;
        case ALARM_OFF:
            break;
        case POLL_REQUEST:
            break;
        case POLL_RESPONSE:
            break;
        case DICP_REQUEST:
            break;
        case DICP_RESPONSE:
            setDoorId(msg.data);
            break;
        case TOL_SET:
            break;
        case ACTIVE_ON:
            break;
        case ACTIVE_OFF:
            break;
    }
}


void main() {
    can_init1(reciever);
}