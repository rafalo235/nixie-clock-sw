/*
 * control-task.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Rafal Olejniczak
 */

#include "control-task/control-task.h"
#include "task.h"
#include <stdint.h>
#include <stdlib.h>
#include "drivers/rtc/rtc.h"
#include "time/time.h"
#include "display/display.h"
#include "stm32f103xb.h"

void IdleState(struct Utils_StateMachine *sm, tUtils_Signal sig);

QueueHandle_t gControlQueue;

static tUtils_StateMachine sStateMachine =
{
		&IdleState, &IdleState
};

void Control_InitializeQueue(void)
{
	gControlQueue = xQueueCreate(5, sizeof(tControlAction));
}

void Control_Task(void *parameters)
{
	tControlAction sig;
	(void)parameters;

	__enable_irq();

	while (1)
	{
		if (pdPASS == xQueueReceive(
				gControlQueue, &sig, portMAX_DELAY))
		{
			Utils_ProcessStateMachine(
					&sStateMachine, sig);
		}
	}
}

void IdleState(struct Utils_StateMachine *sm, tUtils_Signal sig)
{
	switch (sig)
	{
	case CONTROL_ACTION_SECOND_TICK :
	{
		uint32_t tmp = 0;

		if (RTC_SUCCESS == Rtc_Read(&tmp))
		{
			tTime_DateTime t;

			t = Time_GetUTCTime(tmp, NULL);
			Display_Print(t.hour, t.minute);
		}
		break;
	}
	default :
		/* Ignore other signals */
		break;
	}
}
