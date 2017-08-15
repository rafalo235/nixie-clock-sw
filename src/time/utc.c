/*
 * utc.c
 *
 *  Created on: Aug 7, 2017
 *      Author: Rafal Olejniczak
 */

#include "time/time.h"

tTime_DateTime Time_GetUTCTime(uint32_t timestamp, const tTime_DateTime *epoch)
{
	tTime_DateTime utc;
	uint32_t day;
	uint8_t leap;

	utc.hour = (timestamp / SECONDS_PER_HOUR) % HOURS_PER_DAY;
	utc.minute = (timestamp % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
	utc.second = timestamp % SECONDS_PER_HOUR % SECONDS_PER_MINUTE;

	day = timestamp / SECONDS_PER_DAY;

	/* 366 + 3 * 365 = 1461 */
	/* Every epoch year must be multiplicity of 4 ! */
	utc.year = (((day * 4) + 2)/1461);

	leap = !((utc.year + epoch->year) & 3);
	day  -= utc.year * 365 + utc.year / 4;
	day += (day > 58 + leap) ? ((leap) ? 1 : 0) : 0;

	utc.month = ((day * 12) + 6)/367;

	utc.day = day + 1 - ((utc.month * 367) + 5)/12;

	utc.year += epoch->year; /* TODO adding dates */

	return utc;
}
