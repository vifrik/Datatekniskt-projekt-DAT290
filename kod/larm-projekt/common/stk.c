#include "stk.h"
#include "stm32f4xx.h"
#include "core_cm4.h"
#include <stdio.h>

static unsigned char delay_non_blocking_active = 0;
static unsigned int delay_non_blocking = 0;
static unsigned int delay_blocking = 0;
unsigned long sys_time = 0;
Callback systickCB;

void systick_irc_handler() {
	sys_time++;
	if (delay_non_blocking > 0)
		delay_non_blocking--;
	if (delay_blocking > 0)
		delay_blocking--;
	else if (delay_non_blocking == 0 && delay_non_blocking_active && systickCB != NULL) {
		delay_non_blocking_active = 0;
		systickCB();
	}
		
}

// Fördröjning utan att hänga programmet
// För att något ska hända måste först callback_init att kallas
void delay_no_block(unsigned int count) {
	if (!count) return;
	
	delay_non_blocking_active = 1;
	delay_non_blocking = count;
}

void delay(unsigned int count) {
	delay_blocking = count;
	while(delay_blocking > 0);
}

void stk_init(void) {
	//*SCB_VTOR = 0x2001C000;
	SysTick_Config(168);
	
	NVIC_SetPriority(SysTick_IRQn, 0);
	
	*((void (**)(void)) 0x2001C03C) = systick_irc_handler;
}

void systick_callback_init(Callback callback) {
	systickCB = callback;
}