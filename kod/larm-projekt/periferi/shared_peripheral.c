#include "shared_peripheral.h"
#include "can.h"

void state_init(State *state) {
	state->active = 0;
	state->id = 0;
	state->alarm = 0;
	state->tolerance = 0;
}

void canmsg_init(CANMsg *msg, State *state) {
	msg->nodeId = state->id;
	msg->dir = 1;
	msg->length = 0;
	for (int i = 0; i < 8; i++) {
		msg->buff[i] = 0;
	}
}

void alarm_raise(State *state) {
	state.alarm = 1;

	CANMsg msg;
	canmsg_init(&msg, state);
	msg.msgId = ALARM;

	can_send(&msg);
}

void alarm_lower(State *state) {
	state->alarm = 0;
}

void poll_respond(CANMsg *msg, State *state) {
	CANMsg response;
	canmsg_init(&msg, state);
	response.msgId = POLL_RESPONSE;
	response.length = 8;
	for (int i = 0; i < 8; i++) {
		response.buff[i] = ~msg->buff[i];
	}

	can_send(&response);
}

void request_id(State *state) {
	DUMP("Requesting ID");

	CANMsg msg;
	canmsg_init(&msg, state);
	msg.msgId = DICP_REQUEST;

	can_send(&msg);
}

void update_id(CANMsg *msg, State *state) {
	DUMP("Recieved ID");
	DUMP_numeric(msg->buff[0]);
	state->id = msg->buff[0];
}

void update_tolerance(CANMsg *msg, State *state) {
	state->tolerance = msg->buff[0];
}