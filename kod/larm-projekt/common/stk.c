#include "stk.h"
#include "stm32f4xx.h"
#include "core_cm4.h"
#include <stdio.h>

static volatile int delay_count = -1;
unsigned long sys_time = 0;
Callback cb;

void systick_irc_handler() {
	sys_time++;
	if (delay_count > 0)
		delay_count--;
	else if (delay_count == 0 && cb != NULL) {
		delay_count = -1;
		cb();
	}
		
}

// Fördröjning utan att hänga programmet
// För att något ska hända måste först callback_init att kallas
void delay_no_block(unsigned int count) {
	if (!count) return;

	delay_count = count;
}

void delay(unsigned int count) {
	delay_no_block(count);
	while(delay_count > 0);
}

void stk_init(void) {
	//*SCB_VTOR = 0x2001C000;
	SysTick_Config(168);
	
	NVIC_SetPriority(SysTick_IRQn, 0);
	
	*((void (**)(void)) 0x2001C03C) = systick_irc_handler;
}

void callback_init(Callback callback) {
	cb = callback;
}