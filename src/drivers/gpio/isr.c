/*
 * isr.c
 *
 *  Created on: Aug 8, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/gpio/gpio.h"
#include "stm32f103xb.h"

void EXTI1_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR1;
	__asm("nop");
}


void EXTI2_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR2;
	__asm("nop");
}


void EXTI3_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR3;
	__asm("nop");
}
