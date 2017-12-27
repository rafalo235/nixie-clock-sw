/*
 * init.c
 *
 *  Created on: Aug 2, 2017
 *      Author: Rafal Olejniczak
 */

#include "pwm/pwm.h"
#include "stm32f103xb.h"

static void InitializePins(void);
static void InitializeClock(void);

void Pwm_Initialize(void)
{
	InitializePins();
	InitializeClock();
}

static void InitializePins(void)
{
#define CONFIG_ALTERNATIVE_PUSH_PULL	(GPIO_CRH_CNF10_1)
#define MODE_FREQUENCY_LOW	(GPIO_CRH_MODE10_1)
#define REMAP_TIM2_CH3_TO_PB10	(AFIO_MAPR_TIM2_REMAP_1)

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	GPIOB->CRH = (GPIOB->CRH & ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10)) |
			CONFIG_ALTERNATIVE_PUSH_PULL | MODE_FREQUENCY_LOW;
	AFIO->MAPR = (AFIO->MAPR & ~(AFIO_MAPR_TIM2_REMAP)) |
			REMAP_TIM2_CH3_TO_PB10;
}

static void InitializeClock(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	/* Disable clock */
	TIM2->CR1 = 0;

	TIM2->CCMR2 = (TIM2->CCMR2 & ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC3FE)) |
			TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
	TIM2->CCER |= TIM_CCER_CC3E;
	TIM2->PSC = 36;
	TIM2->ARR = 40;
	TIM2->CCR3 = 39;

	/* Enable clock */
	TIM2->CR1 = TIM_CR1_CEN;
}
