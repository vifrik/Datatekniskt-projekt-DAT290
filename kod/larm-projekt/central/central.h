#ifndef CENTRAL_RESPONSE_H
#define CENTRAL_RESPONSE_H

typedef unsigned char uchar;

void init(void);

typedef struct {
	uchar id;
	uchar type;
} Periphery;

#endif

