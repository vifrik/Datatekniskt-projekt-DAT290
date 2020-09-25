#ifndef CENTRAL_RESPONSE_H
#define CENTRAL_RESPONSE_H

typedef unsigned char uchar;

typedef struct {
    uchar devices;
    uchar curr_poll;
    uchar ready;
} State;

typedef struct {
    uchar id;
    uchar type;
    uchar buff[8];
} Peripheral; 

void init(void);

#endif

