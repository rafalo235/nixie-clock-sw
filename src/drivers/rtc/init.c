/*
 * init.c
 *
 *  Created on: Aug 6, 2017
 *      Author: Rafal Olejniczak
 */
#include "drivers/rtc/rtc.h"
#include "stm32f103xb.h"

static void InitializeClock(void);

void Rtc_Initialize(void)
{
	InitializeClock();
}

static void InitializeClock(void)
{
	if (RCC->BDCR !=
			(RCC_BDCR_LSEON | RCC_BDCR_LSERDY |
			RCC_BDCR_RTCSEL_LSE | RCC_BDCR_RTCEN))
	{
		/* Disable write-protection */
		PWR->CR |= PWR_CR_DBP;

		RCC->BDCR |= RCC_BDCR_LSEON;
		while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) { }

		RCC->BDCR = (RCC->BDCR & ~(RCC_BDCR_RTCSEL)) |
				RCC_BDCR_RTCSEL_LSE | RCC_BDCR_RTCEN;

		/* Enable write-protection */
		PWR->CR &= ~(PWR_CR_DBP);
	}
}
