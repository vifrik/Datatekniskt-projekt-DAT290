#ifndef SHARED_PERIPHERAL_H
#define SHARED_PERIPHERAL_H

#include "can.h"

typedef unsigned char uchar;

typedef struct {
	uchar active;
	uchar id;
	uchar alarm;
	uchar tolerance;
} State;

State state;

void state_init();
void canmsg_init(CANMsg *msg);
void alarm_raise();
void alarm_lower();
void poll_respond(CANMsg *msg);
void request_id(uchar type) ;
void update_id(CANMsg *msg);
void update_tolerance(CANMsg *msg);

#endif