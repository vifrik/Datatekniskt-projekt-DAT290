#include "stm32f4xx_gpio.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"

// Required pointers for interupts
#define SYSCFG				0x40013800
#define SYSCFG_EXTICR1		((volatile unsigned int *) (SYSCFG+0x8))

#define EXTI				0x40013C00
#define EXTI_IMR			((volatile unsigned int *) (EXTI))
#define EXTI_FTSR			((volatile unsigned int *) (EXTI+0xC))
#define EXTI_RTSR			((volatile unsigned int *) (EXTI+0x8))
#define EXTI_PR				((volatile unsigned int *) (EXTI+0x14))

#define EXTI0_IRQVEC		((void (**)(void)) 0x2001C058)

#define NVIC_ISER0			((volatile unsigned int *) 0xE000E100)
#define NVIC_EXTI0_IRQ_BPOS	(1 << 6)

#define EXTI0_IRQ_BPOS		1

#define SCB_VTOR 			((volatile unsigned long *) 0xE000ED08)

#define GPIO_Pin_Low               ((uint16_t)0x00FF)  /* All pins selected */
#define GPIO_Pin_High              ((uint16_t)0xFF00)  /* All pins selected */



void mainInit(void){
	GPIO_InitTypeDef init;
	GPIO_StructInit( &init );
	
	init.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;
	init.GPIO_Mode = GPIO_Mode_IN;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_DOWN;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &init);
	
	init.GPIO_Pin = GPIO_Pin_Low;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &init);
	
	init.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 ;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &init);

}

void kbdActivate(unsigned int row) {
    switch(row) {
        case 1: GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET ); break;
        case 2: GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET ); break;
        case 3: GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET ); break;
        case 4: GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET ); break;
        default: GPIO_WriteBit(GPIOD, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12, Bit_RESET);
    }
}

int kbdGetCol(void) {
    unsigned char c;
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
	for(row = 1; row <= 4; row++){
		kbdActivate(row);
		if(col = kbdGetCol() ){
			kbdActivate(0);
			return key[4*(row - 1) + (col - 1)];
		}
	}
	kbdActivate(0);
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

void think(void) {
	mainInit();

    while (1) {
		//GPIO_Write(GPIOD, 0x3F);
        out7seg(keyb());
    }
}