/*
 * init.c
 *
 *  Created on: Aug 8, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/gpio/gpio.h"
#include "stm32f103xb.h"

static void InitializePins(void);
static void InitializeInterrupts(void);

void Gpio_Initialize(void)
{
	InitializePins();
	InitializeInterrupts();
}

static void InitializePins(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
}

static void InitializeInterrupts(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	AFIO->EXTICR[0] = (AFIO->EXTICR[0] &
			~(AFIO_EXTICR1_EXTI1 | AFIO_EXTICR1_EXTI3)) |
			AFIO_EXTICR1_EXTI1_PA | AFIO_EXTICR1_EXTI3_PA;

	EXTI->IMR |= EXTI_IMR_IM1 | EXTI_IMR_IM3;
	EXTI->FTSR |= EXTI_FTSR_FT1 | EXTI_FTSR_FT3;
	EXTI->RTSR |= EXTI_RTSR_RT1;

	NVIC_SetPriority(EXTI1_IRQn, 5);
	NVIC_SetPriority(EXTI3_IRQn, 5);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
}
