#ifndef PROXIMITY_H
#define PROXIMITY_H

typedef unsigned char uchar;

typedef void (*ProximityCallback)(unsigned int);

void proximity_init(void);
unsigned int proximity_read(void);

#endif