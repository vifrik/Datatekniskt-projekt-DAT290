#ifndef LAMP_H
#define LAMP_H

void lamp_init();

void green_lamp_enable(unsigned char door_id);

void green_lamp_disable(unsigned char door_id);

void red_lamp_enable(void);

void red_lamp_disable(void);

#endif