/*
 * idle.c
 *
 *  Created on: Aug 15, 2017
 *      Author: Rafal Olejniczak
 */

#include "control-task/states/states.h"
#include "control-task/control-task.h"
#include "drivers/rtc/rtc.h"

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

