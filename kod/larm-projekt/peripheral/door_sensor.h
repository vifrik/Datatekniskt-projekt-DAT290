#ifndef DOOR_H
#define DOOR_H

void door_init();
unsigned char door_read(unsigned char door_id);
void green_lamp_enable(unsigned char door_id);
void green_lamp_disable(unsigned char door_id);
void red_lamp_enable(void);
void red_lamp_disable(void);

#endif