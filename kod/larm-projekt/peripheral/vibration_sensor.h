#ifndef VIBRATION_H
#define VIBRATION_H

typedef void (*Callback)(void);

void vibration_init(void);
unsigned char vibration_read(void);
void vibration_callback_init(Callback callback);

#endif