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

void DisplayWithPosition(const tDatetime * t, int32_t position)
{

	if (0 == position)
	{
		Display_Print(t->minutes, t->seconds);
	}
	else if (1 == position)
	{
		Display_Print(t->hours, t->minutes);
	}
	else if (2 == position)
	{
		Display_Print(t->date + 1, t->hours);
	}
	else if (3 == position)
	{
		Display_Print(t->month + 1, t->date + 1);
	}
	else if (4 == position)
	{
		Display_Print((uint8_t)(t->year / 100), (uint8_t)(t->year % 100));
	}

}

uint32_t GetPositionDelta(int32_t position)
{
  uint32_t result = 0;
  if (0 == position)
  {
    result = 1;
  }
  else if (1 == position)
  {
    result = 60u;
  }
  else if (2 == position)
  {
    result = 3600u;
  }
  else if (3 == position)
  {
    result = 86400u;
  }
  else if (4 == position)
  {
    result = 31536000u;
  }
  return result;
}
