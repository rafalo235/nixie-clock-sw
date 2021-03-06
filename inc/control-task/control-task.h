/*
 * control-task.h
 *
 *  Created on: Aug 9, 2017
 *      Author: rafal
 */

#ifndef INC_CONTROL_TASK_CONTROL_TASK_H_
#define INC_CONTROL_TASK_CONTROL_TASK_H_

#include "utils/state-machine-framework.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "time/datetime.h"

typedef enum ControlAction
{
	CONTROL_ACTION_INCREMENT = UTILS_USER_SIGNAL,
	CONTROL_ACTION_DECREMENT,
	CONTROL_ACTION_PRESS,
	CONTROL_ACTION_SECOND_TICK,
	CONTROL_ACTION_SHOW_IP
} tControlAction;

typedef struct ControlContext
{
	tUtilsStateMachine sm;
	int32_t position;
	tDatetime timeToSet;
} tControlContext;

extern QueueHandle_t gControlQueue;

void Control_InitializeQueue(void);
void Control_Task(void *parameters);

void DisplayWithPosition(const tDatetime * t, int32_t position);

void IncrementWithDelta(tDatetime * dt, int32_t position);

void DecrementWithDelta(tDatetime * dt, int32_t position);

#endif /* INC_CONTROL_TASK_CONTROL_TASK_H_ */
