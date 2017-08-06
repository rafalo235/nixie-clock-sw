/*
 * read.c
 *
 *  Created on: Aug 6, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/rtc/rtc.h"
#include "stm32f103xb.h"

tRtc_Result Rtc_Read(uint32_t *value)
{
	*value = (uint32_t)RTC->CNTH << 16;
	*value |= RTC->CNTL;
	return RTC_SUCCESS;
}
