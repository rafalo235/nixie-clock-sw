/*
 * epoch.c
 *
 *  Created on: Aug 7, 2017
 *      Author: Rafal Olejniczak
 */

#include "time/time.h"

#define DEFAULT_EPOCH	{ 2000, 00, 00, 00, 00, 00 }

static tTime_DateTime sEpoch = DEFAULT_EPOCH;

void Time_SetEpoch(const tTime_DateTime *epoch)
{
	sEpoch = *epoch;
}

const tTime_DateTime* Time_GetEpoch(void)
{
	return &sEpoch;
}
