/*
 * init.c
 *
 *  Created on: Nov 17, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/clock-source/clock-source.h"
#include "stm32f103xb.h"

static void InitializeHsi(void);
static void InitializePll(void);
static void ConfigurePll(void);

void Pll_Initialize(void)
{
	InitializeHsi();
	ConfigurePll();
}

static void InitializeHsi(void)
{
	if (RCC_CR_HSION != (RCC->CR & RCC_CR_HSION))
	{
		RCC->CR |= RCC_CR_HSION;
	}
	while (RCC_CR_HSIRDY != (RCC->CR & RCC_CR_HSIRDY)) { }
}

static void InitializePll(void)
{
	if (RCC_CR_PLLON != (RCC->CR & RCC_CR_PLLON))
	{
		RCC->CR |= RCC_CR_PLLON;
	}
	while (RCC_CR_PLLRDY != (RCC->CR & RCC_CR_PLLRDY)) { }
}

static void ConfigurePll(void)
{
	/* HSI/2 as PLL source (4 MHz) */
	RCC->CFGR &= ~(RCC_CFGR_PLLSRC);

	/* Clock multiplied by 9 (36 MHz) */
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMULL) |
			RCC_CFGR_PLLMULL9;

	InitializePll();

	/* Set PLL as clock source */
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) |
			RCC_CFGR_SW_1;

	while (RCC_CFGR_SWS_1 != (RCC_CFGR_SWS_1 & RCC->CFGR))
	{

	}

}
