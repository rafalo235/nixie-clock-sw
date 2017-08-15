/*
 * conversion.c
 *
 *  Created on: Aug 15, 2017
 *      Author: Rafal Olejniczak
 */

#include "time/time.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	uint32_t sample = 1502819678 - 946684800;
	tTime_DateTime t;

	t = Time_GetUTCTime(sample, Time_GetEpoch());

	printf("Date is : %04u-%02u-%02u %02u:%02u:%02u\n",
			t.year, t.month, t.day, t.hour, t.minute, t.second);



	return 0;
}
