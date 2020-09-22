#include "usart.h"
#include "stm32f4xx_gpio.h"
#include "stk.h"

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
unsigned volatile long time_end = 0;
unsigned volatile int distance = 0;
extern unsigned long sys_time;

void irq_handler_exti0(void) {
	if (*EXTI_PR & EXTI0_IRQ_BPOS) {
		*EXTI_PR |= EXTI0_IRQ_BPOS;
	
		/*if (*GPIO_E_IDR_LOW & 1) {
			*GPIO_E_ODR_LOW |= (1 << 4);
			*GPIO_E_ODR_LOW &= ~(1 << 4);
				
			count++;
		}*/
		
		DUMP_numeric(sys_time);
		
		time_end = sys_time;
		
		long delta_time = time_end - time_start;
		distance = delta_time/58;
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
	
	*SYSCFG_EXTICR1 &= ~0xF; // Koppla PE1-PE0 till avbrottslina EXTI1-EXTI0
	*SYSCFG_EXTICR1 |= 0x4;
	
	*EXTI_IMR |= 0x1; // Konfigurera EXTI1-EXTI0 för att generera avbrott
	
	*EXTI_FTSR &= ~0x1;
	*EXTI_RTSR |= 0x1; // Konfigurera för avbrott på positiv flank

	*SCB_VTOR = 0x2001C000; // Sätt upp avbrottsvektor
	
	//*EXTI0_IRQVEC = irq_handler_exti0;
	
	*NVIC_ISER0 |= NVIC_EXTI0_IRQ_BPOS;
}

unsigned int proximity_read(void) {
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);
	delay(10);
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
	
	time_start = sys_time;
	
	return distance;
}