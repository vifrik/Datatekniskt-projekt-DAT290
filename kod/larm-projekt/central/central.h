#ifndef CENTRAL_RESPONSE_H
#define CENTRAL_RESPONSE_H

typedef unsigned char uchar;

typedef struct {
    uchar type;
    uchar buff[8];
} PERIPHERAL; 


void init_peripheral(PERIPHERAL *p);


void init(void);

#endif

