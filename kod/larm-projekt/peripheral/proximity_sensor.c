#include "usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stk.h"
#include "misc.h"

// Required pointers for interupts
/*
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
*/
#define EXTI0_IRQVEC		((void (**)(void)) 0x2001C058)

unsigned volatile long time_start = 0;
unsigned volatile int distance = 0;
extern unsigned long sys_time;

void irq_handler_exti0(void) {	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
		unsigned char rising = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
		
		if (rising) {
			time_start = sys_time;
		} else {				
			long delta_time = sys_time - time_start;
			distance = delta_time*3.4/100;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void proximity_init(void) {
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	GPIO_StructInit( &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Connect EXTI Line0 to PE0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Avbrottsvektor
    *((void (**) (void)) EXTI0_IRQVEC) = irq_handler_exti0;
}

unsigned int proximity_read(void) {		
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);
	delay(10);
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
	
	return distance;
}