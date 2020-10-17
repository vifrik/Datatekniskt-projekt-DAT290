#ifndef DOOR_PERIPHERAL_H
#define DOOR_PERIPHERAL_H

typedef struct {
	unsigned char active;
	unsigned char tolerance;
	unsigned char alarm;
	unsigned long opened;
} DoorState;

void door_peripheral_init(void);
void door_peripheral_think(void);

#endif
