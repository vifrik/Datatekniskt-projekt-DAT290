#include "stk.h"

#define STK			0xE000E010
#define STK_CTRL	((volatile unsigned int *) (STK))
#define STK_LOAD	((volatile unsigned int *) (STK+0x4))
#define STK_VAL		((volatile unsigned int *) (STK+0x8))
#define STK_CALIB	((volatile unsigned int *) (STK+0xC))

#define SCB_VTOR 	((volatile unsigned long *) 0xE000ED08)	

static volatile int delay_count;
unsigned long sys_time = 0;

void delay_1micro(void) {
	*STK_CTRL = 0;
	*STK_LOAD = 168 - 1;
	*STK_VAL = 0;
	*STK_CTRL = 7;
}

void systick_irc_handler(void) {
	sys_time++;
	if (delay_count > 0)
		delay_count--;
	delay_1micro();
}

void delay(unsigned int count) {
	if (count == 0) return;
	delay_count = count;
	
	delay_1micro();
	
	while(delay_count > 0);
}

void stk_init(void) {
	*SCB_VTOR = 0x2001C000;
	
	*((void (**)(void)) 0x2001C03C) = systick_irc_handler;
}