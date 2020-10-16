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
	uchar alarm;
    uchar buff[8];
	uchar units;
} Peripheral; 

void init(void);
void think(void);

#endif

