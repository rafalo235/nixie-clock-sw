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

void IncrementWithDelta(tDatetime * dt, int32_t position)
{
  int carry = 0;
  uint32_t delta;
  if (0 == position)
  {
    delta = 1u;
    Datetime_IncrementTime(dt, &delta, &carry);
  }
  else if (1 == position)
  {
    delta = 60u;
    Datetime_IncrementTime(dt, &delta, &carry);
  }
  else if (2 == position)
  {
    delta = 3600u;
    Datetime_IncrementTime(dt, &delta, &carry);
  }
  else if (3 == position)
  {
    Datetime_IncrementDay(dt);
  }
  else if (4 == position)
  {
    Datetime_IncrementYear(dt);
  }
}

void DecrementWithDelta(tDatetime * dt, int32_t position)
{
  int carry = 0;
  uint32_t delta;
  if (0 == position)
  {
    delta = 1u;
    Datetime_DecrementTime(dt, &delta, &carry);
  }
  else if (1 == position)
  {
    delta = 60u;
    Datetime_DecrementTime(dt, &delta, &carry);
  }
  else if (2 == position)
  {
    delta = 3600u;
    Datetime_DecrementTime(dt, &delta, &carry);
  }
  else if (3 == position)
  {
    Datetime_DecrementDay(dt);
  }
  else if (4 == position)
  {
    Datetime_DecrementYear(dt);
  }
}
