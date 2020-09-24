#ifndef SHARED_PERIPHERAL_H
#define SHARED_PERIPHERAL_H

typedef unsigned char uchar;

typedef struct {
	uchar active;
	uchar id;
	uchar alarm;
	uchar tolerance;
} State;

void state_init(State *state);
void canmsg_init(CANMsg *msg, State *state);
void alarm_raise(State *state)M
void alarm_lower(State *state);
void poll_respond(CANMsg *msg, State *state);
void request_id(State *state);
void update_id(CANMsg *msg, State *state);
void update_tolerance(CANMsg *msg, State *state);

#endif