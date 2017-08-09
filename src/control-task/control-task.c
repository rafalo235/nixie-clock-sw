/*
 * control-task.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Rafal Olejniczak
 */

#include "control-task/control-task.h"
#include "utils/state-machine-framework.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdlib.h>
#include "drivers/i2c/i2c.h"
#include "drivers/rtc/rtc.h"
#include "time/time.h"
#include "utils/coding.h"

enum ControlTaskSignal
{
	CONTROL_TASK_SECOND_TICK = UTILS_USER_SIGNAL
};

void IdleState(struct Utils_StateMachine *sm, tUtils_Signal sig);

static tUtils_StateMachine sStateMachine =
{
		&IdleState, &IdleState
};

void Control_Task(void *parameters)
{
	(void)parameters;

	while (1)
	{
		Utils_ProcessStateMachine(&sStateMachine, CONTROL_TASK_SECOND_TICK);
	}
}

void IdleState(struct Utils_StateMachine *sm, tUtils_Signal sig)
{
	switch (sig)
	{
	case CONTROL_TASK_SECOND_TICK :
	{
		uint32_t tmp = 0;

#define SWAP_N_BITS(x,n)	(((x) << (n)) | ((x) >> (n)))

		if (RTC_SUCCESS == Rtc_Read(&tmp))
		{
			uint8_t hours = 0, minutes = 0;
			tTime_DateTime t;

			t = Time_GetUTCTime(tmp, NULL);

			t.minute = Utils_BinToBcd(t.minute);
			t.hour = Utils_BinToBcd(t.hour);

			minutes = SWAP_N_BITS(t.minute, 4);
			hours = SWAP_N_BITS(t.hour, 4);

			I2C_Write(0x70, &minutes, 1);
			I2C_Write(0x72, &hours, 1);
		}
		break;
	}
	default :
		/* Ignore other signals */
		break;
	}
}
