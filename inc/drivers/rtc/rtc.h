/*
 * rtc.h
 *
 *  Created on: Aug 6, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_DRIVERS_RTC_RTC_H_
#define INC_DRIVERS_RTC_RTC_H_

#include <stdint.h>

typedef enum Rtc_Result {
	RTC_SUCCESS
} tRtc_Result;

void Rtc_Initialize(void);
tRtc_Result Rtc_Read(uint32_t *value);

#endif /* INC_DRIVERS_RTC_RTC_H_ */
