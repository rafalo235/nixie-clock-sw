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
void SetTimeState(void *sm, tUtils_Signal sig);

static void DisplayWithPosition(uint32_t sec, int32_t position);
static uint32_t IncrementWithPosition(uint32_t sec, int32_t position);
static uint32_t DecrementWithPosition(uint32_t sec, int32_t position);

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
	uint32_t tmp = 0;

	switch (sig)
	{
	case CONTROL_ACTION_SECOND_TICK :
	{
		if (RTC_SUCCESS == Rtc_Read(&tmp))
		{
			DisplayWithPosition(tmp, context->position);
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

		if (RTC_SUCCESS == Rtc_Read(&tmp))
		{
			DisplayWithPosition(tmp, context->position);
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

		if (RTC_SUCCESS == Rtc_Read(&tmp))
		{
			DisplayWithPosition(tmp, context->position);
		}
		break;
	}
	case CONTROL_ACTION_PRESS :
	{

		STATE_CHANGE(context, &SetTimeState);
		break;
	}
	default :
		/* Ignore other signals */
		break;
	}
}

void SetTimeState(void *sm, tUtils_Signal sig)
{
	tControlContext *context = sm;

	switch (sig)
	{
	case UTILS_ENTER_STATE :
	{
		context->timeToSet = 0;
		Rtc_Read(&(context->timeToSet));
		DisplayWithPosition(context->timeToSet, context->position);
		break;
	}
	case CONTROL_ACTION_INCREMENT :
	{
		context->timeToSet =
				IncrementWithPosition(context->timeToSet, context->position);
		DisplayWithPosition(context->timeToSet, context->position);
		break;
	}
	case CONTROL_ACTION_DECREMENT :
	{
		context->timeToSet =
				DecrementWithPosition(context->timeToSet, context->position);
		DisplayWithPosition(context->timeToSet, context->position);
		break;
	}
	case CONTROL_ACTION_PRESS :
	{
		STATE_CHANGE(context, &IdleState);
		break;
	}
	case UTILS_LEAVE_STATE :
	{
		/* TODO set time */
		break;
	}
	default :
		/* Ignore other signals */
		break;
	}

}

static void DisplayWithPosition(uint32_t sec, int32_t position)
{
	tTime_DateTime t;

	t = Time_GetUTCTime(sec, NULL);
	if (0 == position)
	{
		Display_Print(t.minute, t.second);
	}
	else if (1 == position)
	{
		Display_Print(t.hour, t.minute);
	}
	else if (2 == position)
	{
		Display_Print(t.day, t.hour);
	}
	else if (3 == position)
	{
		Display_Print(t.month, t.day);
	}
	else if (4 == position)
	{
		Display_Print((uint8_t)(t.year / 100), (uint8_t)t.year);
	}

}

static uint32_t IncrementWithPosition(uint32_t sec, int32_t position)
{
	uint32_t t = sec;
	if (0 == position)
	{
		++t;
	}
	else if (1 == position)
	{
		t += 60u;
	}
	else if (2 == position)
	{
		t += 3600u;
	}
	else if (3 == position)
	{
		t += 86400u;
	}
	else if (4 == position)
	{
		t += 31536000u;
	}
}

static uint32_t DecrementWithPosition(uint32_t sec, int32_t position)
{
	uint32_t t = sec;
	if (0 == position)
	{
		--t;
	}
	else if (1 == position)
	{
		t -= 60u;
	}
	else if (2 == position)
	{
		t -= 3600u;
	}
	else if (3 == position)
	{
		t -= 86400u;
	}
	else if (4 == position)
	{
		t -= 31536000u;
	}
}
