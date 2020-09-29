#include "shared_peripheral.h"
#include "messages.h"

void state_init() {
	state.active = 0;
	state.id = 0xF;
	state.alarm = 0;
	state.tolerance = 0;
}

void canmsg_init(CANMsg *msg) {
	msg->nodeId = state.id;
	msg->dir = TO_CENTRAL;
	msg->length = 0;
	for (int i = 0; i < 8; i++) {
		msg->buff[i] = 0;
	}
}

void alarm_raise() {
	state.alarm = 1;

	CANMsg msg;
	canmsg_init(&msg);
	msg.msgId = ALARM;

	can_send(&msg);
}

void alarm_lower() {
	DUMP("Alarm off");
	state.alarm = 0;
}

void poll_respond(CANMsg *msg) {
	CANMsg response;
	canmsg_init(&response);
	response.msgId = POLL_RESPONSE;
	response.length = 8;
	for (int i = 0; i < 8; i++) {
		response.buff[i] = ~msg->buff[i];
	}

	can_send(&response);
	
}

void request_id(uchar type) {
	DUMP("Requesting ID");

	CANMsg msg;
	canmsg_init(&msg);
	msg.msgId = DICP_REQUEST;
	msg.length = 1;
	msg.buff[0] = type;

	can_send(&msg);
}

void update_id(CANMsg *msg) {
	DUMP("Recieved ID");
	DUMP_numeric(msg->buff[0]);
	state.id = msg->buff[0];
}

void update_tolerance(CANMsg *msg) {
	state.tolerance = msg->buff[0];
}