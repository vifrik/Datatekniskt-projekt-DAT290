#ifndef CENTRAL_RESPONSE_H
#define CENTRAL_RESPONSE_H

typedef unsigned char uchar;

typedef struct {
	uchar alarm;
    uchar devices;
    uchar curr_poll;
    uchar ready;
} State;

typedef struct {
    uchar type;
    uchar buff[8];
} Peripheral; 

void init(void);

#endif

