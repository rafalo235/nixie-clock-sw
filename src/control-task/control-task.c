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
#include "time/time.h"
#include "display/display.h"
#include "stm32f103xb.h"
#include "control-task/states/states.h"

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

void DisplayWithPosition(uint32_t sec, int32_t position)
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

uint32_t IncrementWithPosition(uint32_t sec, int32_t position)
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

uint32_t DecrementWithPosition(uint32_t sec, int32_t position)
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
