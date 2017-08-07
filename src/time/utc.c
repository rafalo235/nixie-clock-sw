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

	utc.hour = (timestamp / 3600) % 24;
	utc.minute = (timestamp % 3600) / 60;
	utc.second = timestamp % 3600 % 60;

	return utc;
}
