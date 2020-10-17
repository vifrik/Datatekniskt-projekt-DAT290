#include <stdlib.h>
#include "vibration_sensor.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#define EXTI2_IRQVEC		((void (**)(void)) 0x2001C060)

Callback vibCB;

// Avbrottshanterare
void irq_handler_exti2(void) {
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) {
		if (vibCB != NULL) {
			vibCB();
		}
		DUMP("Vibration");
		
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void vibration_init(void) {
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
    /* Tell system that you will use PE2 for EXTI_Line2 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
    
    /* PE2 is connected to EXTI_Line2 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    /* Add IRQ vector to NVIC */
    /* PE2 is connected to EXTI_Line2, which has EXTI2_IRQn vector */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0E;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0E;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	// Avbrottsvektor
    *((void (**) (void)) EXTI2_IRQVEC) = irq_handler_exti2;
}

void vibration_callback_init(Callback callback) {
	vibCB = callback;
}