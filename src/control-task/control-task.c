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

void IdleState(void *sm, tUtils_Signal sig);

QueueHandle_t gControlQueue;

void Control_InitializeQueue(void)
{
	gControlQueue = xQueueCreate(5, sizeof(tControlAction));
}

void Control_Task(void *parameters)
{
	tControlContext controlContext =
	{
			{ &IdleState, &IdleState },
			1
	};
	tControlAction sig;
	(void)parameters;

	__enable_irq();

	while (1)
	{
		if (pdPASS == xQueueReceive(
				gControlQueue, &sig, portMAX_DELAY))
		{
			Utils_ProcessStateMachine(
					&controlContext, sig);
		}
	}
}

void IdleState(void *sm, tUtils_Signal sig)
{
	tControlContext *context = sm;

	switch (sig)
	{
	case CONTROL_ACTION_SECOND_TICK :
	{
		uint32_t tmp = 0;

		if (RTC_SUCCESS == Rtc_Read(&tmp))
		{
			tTime_DateTime t;

			t = Time_GetUTCTime(tmp, NULL);
			if (0 == context->position)
			{
				Display_Print(t.minute, t.second);
			}
			else if (1 == context->position)
			{
				Display_Print(t.hour, t.minute);
			}
			else if (2 == context->position)
			{
				Display_Print(t.day, t.hour);
			}
			else if (3 == context->position)
			{
				Display_Print(t.month, t.day);
			}
			else if (4 == context->position)
			{
				Display_Print((uint8_t)(t.year / 100), (uint8_t)t.year);
			}
		}
		break;
	}
	case CONTROL_ACTION_INCREMENT :
	{
		++(context->position);
		if (context->position > 4)
		{
			context->position = 4;
		}
		break;
	}
	case CONTROL_ACTION_DECREMENT :
	{
		--(context->position);
		if (context->position < 0)
		{
			context->position = 0;
		}
		break;
	}
	default :
		/* Ignore other signals */
		break;
	}
}
