/*
 * time.h
 *
 *  Created on: Aug 7, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_TIME_TIME_H_
#define INC_TIME_TIME_H_

#include <stdint.h>

#define HOURS_PER_DAY	(24u)
#define MINUTES_PER_HOUR	(60u)
#define SECONDS_PER_MINUTE	(60u)
#define SECONDS_PER_HOUR	(3600u)
#define SECONDS_PER_DAY		(86400u)

typedef struct Time_DateTime {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} tTime_DateTime;

void RTC_IRQHandler(void);

#endif /* INC_TIME_TIME_H_ */
