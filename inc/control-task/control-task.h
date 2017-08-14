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

typedef enum ControlAction
{
	CONTROL_ACTION_INCREMENT = UTILS_USER_SIGNAL,
	CONTROL_ACTION_DECREMENT,
	CONTROL_ACTION_PRESS,
	CONTROL_ACTION_SECOND_TICK
} tControlAction;

typedef struct ControlContext
{
	tUtilsStateMachine sm;
	int32_t position;
	uint32_t timeToSet;
} tControlContext;

extern QueueHandle_t gControlQueue;

void Control_InitializeQueue(void);
void Control_Task(void *parameters);

#endif /* INC_CONTROL_TASK_CONTROL_TASK_H_ */
