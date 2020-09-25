#ifndef STK_H
#define STK_H

typedef void (*Callback)(void);

void delay_no_block(unsigned int count);
void delay(unsigned int count);
void stk_init(void);
void callback_init(Callback callback);

#endif