/*
 * write.c
 *
 *  Created on: Aug 17, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/rtc/rtc.h"
#include "stm32f103xb.h"

tRtc_Result Rtc_Write(uint32_t value)
{
	while ((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF) { }

	/* Disable write-protection */
	PWR->CR |= PWR_CR_DBP;

	while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF) { }

	RTC->CRL |= RTC_CRL_CNF;

	RTC->CNTL = (uint16_t)value;
	RTC->CNTH = (uint16_t)(value >> 16);

	RTC->CRL &= ~RTC_CRL_CNF;

	while ((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF) { }

	/* Enable write-protection */
	PWR->CR &= ~PWR_CR_DBP;

	return RTC_SUCCESS;
}
