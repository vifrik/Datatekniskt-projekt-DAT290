#include "shared_peripheral.h"
#include "messages.h"
#include "lamp.h"

void state_init() {
	state.active = 0;
	state.id = 0xF;
	state.alarm = 0;
}

void canmsg_init(CANMsg *msg) {
	msg->nodeId = state.id;
	msg->dir = TO_CENTRAL;
	msg->length = 0;
	for (int i = 0; i < 8; i++) {
		msg->buff[i] = 0;
	}
}

void alarm_raise(uchar unitId) {
	usart_sendl("Alarm");
	red_lamp_enable();
	
	state.alarm = 1;

	CANMsg msg;
	canmsg_init(&msg);
	msg.msgId = ALARM;
	msg.length = 1;
	msg.buff[0] = unitId;

	can_send(&msg);
}

void alarm_lower() {
	red_lamp_disable();
	usart_sendl("Alarm off");
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

void request_id(uchar type, uchar units) {
	usart_sendl("Requesting ID");

	CANMsg msg;
	canmsg_init(&msg);
	msg.msgId = DICP_REQUEST;
	msg.length = 2;
	msg.buff[0] = type;
	msg.buff[1] = units;

	can_send(&msg);
}

void update_id(CANMsg *msg) {
	usart_send("Recieved ID:");
	usart_send_numericl(msg->buff[0]);
	state.id = msg->buff[0];
}