/*
 * time.h
 *
 *  Created on: Aug 7, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_TIME_TIME_H_
#define INC_TIME_TIME_H_

#include <stdint.h>

typedef struct Time_DateTime {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} tTime_DateTime;

void Time_SetEpoch(const tTime_DateTime *);
tTime_DateTime Time_GetEpoch(void);

tTime_DateTime Time_GetUTCTime(
		uint32_t timestamp, const tTime_DateTime *epoch);

#endif /* INC_TIME_TIME_H_ */
