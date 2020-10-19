#include "usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stk.h"
#include "misc.h"
#include "proximity_sensor.h"

#define EXTI0_IRQVEC		((void (**)(void)) 0x2001C058)

ProximityCallback proximityCallback;

unsigned long time_start = 0;
extern unsigned long sys_time;

// Avbrottshanterare
void irq_handler_exti0(void) {	
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
		unsigned char rising = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
		
		if (rising) {
			time_start = sys_time;
		} else {				
			long delta_time = sys_time - time_start;
			proximityCallback(delta_time*3.4/100);
		}
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

// Initialisering av GPIO och avbrott
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Avbrottsvektor
    *((void (**) (void)) EXTI0_IRQVEC) = irq_handler_exti0;
}

void proximity_callback_init(ProximityCallback pc) {
	proximityCallback = pc;
}

// Skriver 1 och sedan 0 på trig med 10us fördröjning, returnerar avstånd
void proximity_read(void) {		
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_SET);
	delay(10);
	GPIO_WriteBit(GPIOE, GPIO_Pin_1, Bit_RESET);
}