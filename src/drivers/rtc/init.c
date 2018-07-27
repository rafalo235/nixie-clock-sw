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

static int sInitialized = 0;

tDatetime sDatetime;

void Rtc_Initialize(void)
{
	InitializeClock();
	sInitialized = 1;
}

void Rtc_GetDatetime(tDatetime * dt)
{
  /* todo is padding need to be considered? */
  uint16_t * dst = (uint16_t *)dt;
  const uint16_t * src = (uint16_t *)&BKP->DR1;
  size_t len = sizeof(tDatetime) / sizeof(uint16_t);

  if (1 == sInitialized)
  {
    __disable_irq();

    while (len--)
    {
      *dst = *src;
      ++dst;
      src += 2;
    }
    __enable_irq();
  }
}

void Rtc_SetDatetime(const tDatetime * dt)
{
  /* todo is padding need to be considered? */
  uint16_t * dst = (uint16_t *)&BKP->DR1;
  const uint16_t * src = (uint16_t *)dt;
  size_t len = sizeof(tDatetime) / sizeof(uint16_t);


  if (1 == sInitialized)
  {
    __disable_irq();
    PWR->CR |= PWR_CR_DBP;

    while (len--)
    {
      *dst = *src;
      dst += 2;
      ++src;
    }

    PWR->CR &= ~(PWR_CR_DBP);
    __enable_irq();
  }
}

static void InitializeClock(void)
{
	/* 32767 + 1 Hz -> 1 s  */
#define PRESCALER_VALUE		32767
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN;

	/* Disable write-protection */
	PWR->CR |= PWR_CR_DBP;

	if (RCC->BDCR !=
			(RCC_BDCR_LSEON | RCC_BDCR_LSERDY |
			RCC_BDCR_RTCSEL_LSE | RCC_BDCR_RTCEN))
	{

		RCC->BDCR |= RCC_BDCR_LSEON;
		while ((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY) { }

		RCC->BDCR = (RCC->BDCR & ~(RCC_BDCR_RTCSEL)) |
				RCC_BDCR_RTCSEL_LSE | RCC_BDCR_RTCEN;

		/* Poll until RTC registers are synchronized */
		while ((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF) { }

		/* Poll until RTC operations are ongoing */
		while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF) { }

		RTC->PRLL = PRESCALER_VALUE;
	}

	/* Enable interrupts */
	InitializeInterrupts();
	RTC->CRH |= RTC_CRH_SECIE;

	while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF) { }

	/* Enable write-protection */
	PWR->CR &= ~(PWR_CR_DBP);
}

static void InitializeInterrupts(void)
{
	NVIC_SetPriority(RTC_IRQn, 5);
	NVIC_EnableIRQ(RTC_IRQn);
}
