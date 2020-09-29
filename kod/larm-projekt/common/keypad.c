#include "stm32f4xx_gpio.h"
#include "usart.h"
#include "stk.h"
//#include "stm32f4xx_rcc.h"

#define GPIO_Pin_Low               ((uint16_t)0x00FF)  /* All pins selected */
#define GPIO_Pin_High              ((uint16_t)0xFF00)  /* All pins selected */

void keyboard_init(void) {
	GPIO_InitTypeDef init;
	GPIO_StructInit( &init );
	
	init.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;
	init.GPIO_Mode = GPIO_Mode_IN;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_DOWN;
	init.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &init);
	
	init.GPIO_Pin = GPIO_Pin_Low;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	init.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &init);
	
	init.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 ;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	init.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &init);

}

void activate_row(unsigned int row) {
	GPIO_WriteBit(GPIOD, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12, Bit_RESET);
    switch(row) {
        case 1: GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET ); break;
        case 2: GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET ); break;
        case 3: GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET ); break;
        case 4: GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET ); break;
    }
}

int get_column(void) {
    unsigned char c;
	//c = GPIO_ReadInputData(GPIOD) >> 8 & 0xF;
	c = (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11))*8 | (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10))*4 | (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9))*2 | GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
    if (c & 8) return 4;
    if (c & 4) return 3;
    if (c & 2) return 2;
    if (c & 1) return 1;
    return 0;
}

unsigned char keyb(void) {
	char key[] = {1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};
	int row,col;
	for (row = 1; row <= 4; row++) {
		activate_row(row);
		if (col = get_column()) {
			activate_row(0);
			return key[4*(row - 1) + (col - 1)];
		}
	}
	activate_row(0);
	return 0xFF;
}

void out7seg(unsigned char c) {
	char segCodes[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
	if(c != 0xFF){
		GPIO_Write(GPIOD, segCodes[c]);
	} else {
		GPIO_Write(GPIOD, 0);
	}
}

unsigned char pressed = 0;
extern unsigned long sys_time;
unsigned char key;

void keyboard_input(unsigned char input[]) {
	key = keyb();
	out7seg(key);

	if (!pressed && key <= 0xF && key >= 1) {
		pressed = 1;

		// Shift all values left
		for (int i = 0; i < 3; i++) {
			input[i] = input[i+1];
		}

		input[3] = key;
	} else if (key == 0xFF) {
		pressed = 0;
	}
}