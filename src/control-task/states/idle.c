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
	tDatetime tmp;

	switch (sig)
	{
	case CONTROL_ACTION_SECOND_TICK :
	{
	  Rtc_GetDatetime(&tmp);
    DisplayWithPosition(&tmp, context->position);
		break;
	}
	case CONTROL_ACTION_INCREMENT :
	{
		++(context->position);
		if (context->position > 4)
		{
			context->position = 4;
		}

    Rtc_GetDatetime(&tmp);
    DisplayWithPosition(&tmp, context->position);
		break;
	}
	case CONTROL_ACTION_DECREMENT :
	{
		--(context->position);
		if (context->position < 0)
		{
			context->position = 0;
		}

    Rtc_GetDatetime(&tmp);
    DisplayWithPosition(&tmp, context->position);
		break;
	}
	case CONTROL_ACTION_PRESS :
	{
		STATE_CHANGE(context, &SetTimeState);
		break;
	}
	case CONTROL_ACTION_SHOW_IP :
	{
    STATE_CHANGE(context, &ShowIpState);
	  break;
	}
	default :
		/* Ignore other signals */
		break;
	}
}

