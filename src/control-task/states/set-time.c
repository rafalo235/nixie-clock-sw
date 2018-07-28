/*
 * set-time.c
 *
 *  Created on: Aug 15, 2017
 *      Author: Rafal Olejniczak
 */

#include "control-task/states/states.h"
#include "control-task/control-task.h"
#include "drivers/rtc/rtc.h"

void SetTimeState(void *sm, tUtils_Signal sig)
{
	tControlContext *context = sm;

	switch (sig)
	{
	case UTILS_ENTER_STATE :
	{
	  Rtc_GetDatetime(&(context->timeToSet));
    DisplayWithPosition(&(context->timeToSet), context->position);
		break;
	}
	case CONTROL_ACTION_INCREMENT :
	{
    IncrementWithDelta(&(context->timeToSet), context->position);
		DisplayWithPosition(&(context->timeToSet), context->position);
		break;
	}
	case CONTROL_ACTION_DECREMENT :
	{
    DecrementWithDelta(&(context->timeToSet), context->position);
    DisplayWithPosition(&(context->timeToSet), context->position);
		break;
	}
	case CONTROL_ACTION_PRESS :
	{
		STATE_CHANGE(context, &IdleState);
		break;
	}
	case UTILS_LEAVE_STATE :
	{
	  Rtc_SetDatetime(&(context->timeToSet));
		break;
	}
	default :
		/* Ignore other signals */
		break;
	}
}
