#ifndef SHARED_PERIPHERAL_H
#define SHARED_PERIPHERAL_H

#include "can.h"

typedef unsigned char uchar;

typedef struct {
	uchar active;
	uchar id;
	uchar alarm;
} State;

State state;

void state_init();
void canmsg_init(CANMsg *msg);
void alarm_raise(uchar unitId);
void alarm_lower();
void poll_respond(CANMsg *msg);
void request_id(uchar type, uchar units);
void update_id(CANMsg *msg);

#endif