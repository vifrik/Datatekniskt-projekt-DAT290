#ifndef MESSAGES_H
#define MESSAGES_H

/*

// Triggers an alarm when recieved on the central device
// Borde göra så att den skickar vilken dörr på dörrenheten / från vilken rörelse sensor?
ALARM:
msgId = ALARM
nodeId = sender id
dir = 0
length = 1;
data = door id / 

ERROR:
msgId = ERROR
nodeId = peripheral
dir = 0/1
length = 1
data = error code

// Disables an alarm on a peripheral device
ALARM_OFF:
msgId = ALARM_OFF
nodeId = reciever id
dir = 1

// Perform a poll reqeust from a peripheral device with id nodeId
POLL_REQUEST:
msgId = POLL_REQUEST
nodeId = reciever id
dir = 0
length = 2
data = arbitrary sequency of bits of length 16

// Respond to poll request
POLL_RESPONSE:
msgId = POLL_RESPONSE
nodeId = sender id
dir = 1
length = 2
data = bitwise complement of request data 

// Dynamic ID Configuration Protocol request, request an ID from the central device
// Borde skicka med underenheter också så att centralenhet kan lägga till dem.
DICP_REQUEST:
msgId = DICP_REQUEST
nodeId = 0
dir = 0
length = 2
data[0] = PERIPHERAL_TYPE
data[1] = amount of subunits

// Respond to request with ID
DICP_RESPONSE:
msgId = DICP_RESPONSE
nodeId = 0x0
dir = 1
lenght = 1
data = id

// Set tolerance in peripheral device with id nodeId
TOL_SET:
msgId = TOL_SET
nodeId = reciever id
dir = 1
length = 1
data = tolerance

// Activate a periferal device
ACTIVE_ON:
msgId = ACTIVE_ON
nodeId = reciever id
dir = 1

// Disable a peripheral device
ACTIVE_OFF:
msgId = ACTIVE_OFF
nodeId = reciever id
dir = 1

*/

enum MSG_ID {
	ALARM,
	ERROR,
	ALARM_OFF,
	POLL_REQUEST,
	POLL_RESPONSE,
	DICP_REQUEST,
	DICP_RESPONSE,
	TOL_SET,
	DOORS_SET,
	ACTIVATE, 
	ACTIVE_ON,
	ACTIVE_OFF
};

enum PERIPHERAL_TYPE {
    DOOR,
    PROXIMITY
};

enum ERROR_CODE {
	MAX_DEVICES
};

enum DIRECTION {
	TO_PERIPHERAL,
	TO_CENTRAL
};

#endif