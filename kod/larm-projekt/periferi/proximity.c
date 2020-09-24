#include "usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stk.h"
#include "misc.h"

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

unsigned volatile long time_start = 0;
unsigned volatile int distance = 0;
extern unsigned long sys_time;

void irq_handler_exti0(void) {
	if (*EXTI_PR & EXTI0_IRQ_BPOS) {
		*EXTI_PR |= EXTI0_IRQ_BPOS;
	
		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) {
			
			DUMP("sys_time");
			DUMP_numeric(sys_time);
			DUMP_numeric(time_start);
			
			long delta_time = sys_time - time_start;
			distance = delta_time/58;
			
			DUMP_numeric(distance);
		}
	}
}

void proximity_init(void) {
	GPIO_InitTypeDef init;
	GPIO_StructInit( &init );
	
	init.GPIO_Pin = GPIO_Pin_0;
	init.GPIO_Mode = GPIO_Mode_IN;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_UP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &init);
	
	init.GPIO_Pin = GPIO_Pin_1;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &init);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	
	// Configure EXTI Line0
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// Avbrottsvektor
    *((void (**) (void)) EXTI0_IRQVEC) = irq_handler_exti0;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
}

unsigned int proximity_read(void) {	
	time_start = sys_time;
	
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);
	delay(10);
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
	
	return distance;
}