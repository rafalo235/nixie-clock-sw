/*
 * init.c
 *
 *  Created on: Aug 6, 2017
 *      Author: Rafal Olejniczak
 */
#include "drivers/rtc/rtc.h"
#include "stm32f103xb.h"

static void InitializeClock(void);
static void InitializeInterrupts(void);

void Rtc_Initialize(void)
{
	InitializeClock();
}

static void InitializeClock(void)
{
	/* 32767 + 1 Hz -> 1 s  */
#define PRESCALER_VALUE		32767
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN;

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

		/* Poll until RTC registers are synchronized */
		while ((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF) { }
		/* Poll until RTC operations are ongoing */
		while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF) { }

		RTC->PRLL = PRESCALER_VALUE;

		/* Enable write-protection */
		PWR->CR &= ~(PWR_CR_DBP);
	}

	/* Enable interrupts */
	InitializeInterrupts();
	RTC->CRH |= RTC_CRH_SECIE;
}

static void InitializeInterrupts(void)
{
	NVIC_SetPriority(RTC_IRQn, 5);
	NVIC_EnableIRQ(RTC_IRQn);
}
