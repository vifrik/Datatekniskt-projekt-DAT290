#include "stm32f4xx_gpio.h"

void door_init() {
	GPIO_InitTypeDef init;
	GPIO_StructInit( &init );
	
	//RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	
	init.GPIO_Pin = GPIO_Pin_0;
	init.GPIO_Mode = GPIO_Mode_IN;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_UP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &init);
}

unsigned char door_read() {
	return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
}