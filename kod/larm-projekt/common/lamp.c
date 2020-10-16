#include "lamp.h"
#include "stm32f4xx_gpio.h"


// Initialiserar GPIO
void lamp_init() {
	GPIO_InitTypeDef init;
	GPIO_StructInit( &init );
	

	// en grön lampa per dörrsensor
	// en röd lampa
	// 15 pinnar totalt
	init.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_UP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &init);
}

void green_lamp_enable(unsigned char door_id) {
	GPIO_WriteBit(GPIOE, 2 << (door_id * 2), Bit_SET);
}

void green_lamp_disable(unsigned char door_id) {
	GPIO_WriteBit(GPIOE, 2 << (door_id * 2), Bit_RESET);
}

void red_lamp_enable(void) {
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_SET);
}

void red_lamp_disable(void) {
	GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_RESET);
}