#include "stk.h"
#include "stm32f4xx.h"
#include "core_cm4.h"

static volatile int delay_count;
unsigned long sys_time = 0;

void systick_irc_handler(void) {
	sys_time++;
	if (delay_count > 0)
		delay_count--;
}

void delay(unsigned int count) {
	if (count == 0) return;
	
	delay_count = count;
	
	while(delay_count > 0);
}

void stk_init(void) {
	//*SCB_VTOR = 0x2001C000;
	SysTick_Config(168);
	
	NVIC_SetPriority(SysTick_IRQn, 0);
	
	*((void (**)(void)) 0x2001C03C) = systick_irc_handler;
}