#ifndef MESSAGES_H
#define MESSAGES_H

typedef struct { //Struct for Door Sensor alarm message
	uchar dir;
    uchar msgId;
    uchar nodeId;
} msgDoorAlarm;

typedef struct { //Struct for Motion Sensor alarm message
	uchar dir;
	uchar msgId;
	uchar nodeId;
} msgMSAlarm;

typedef struct { //Struct for central->periphery unit Polling msg
	uchar msgId;
} msgPoll;

#endif