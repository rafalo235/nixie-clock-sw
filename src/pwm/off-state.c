/*
 * off-state.c
 *
 *  Created on: Nov 19, 2017
 *      Author: Rafal Olejniczak
 */

#include "pwm/pwm.h"
#include "stm32f103xb.h"

static void InitializePins(void);
static void SetLowState(void);

void Pwm_InitializeOffState(void)
{
	InitializePins();
	SetLowState();
}

static void InitializePins(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	/* Output 2MHz, Push-pull */
	GPIOB->CRH = (GPIOB->CRH & ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10)) |
			GPIO_CRH_MODE10_1;
}

static void SetLowState(void)
{
	GPIOB->BRR = GPIO_BRR_BR10;
}
