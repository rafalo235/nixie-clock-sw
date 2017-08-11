/*
 * isr.c
 *
 *  Created on: Aug 10, 2017
 *      Author: Rafal Olejniczak
 */

#include "time/time.h"
#include "stm32f103xb.h"
#include "control-task/control-task.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdlib.h>

void RTC_IRQHandler(void)
{
	const tControlAction action = CONTROL_ACTION_SECOND_TICK;
	BaseType_t higherTaskPriority = pdFALSE;

	/* Clear pending interrupt */
	//EXTI->PR = EXTI_PR_PR17;
	RTC->CRL &= ~RTC_CRL_SECF;
	//NVIC_ClearPendingIRQ(RTC_IRQn);


	if (pdFALSE == xQueueIsQueueFullFromISR(gControlQueue))
	{
		xQueueSendToBackFromISR(
				gControlQueue, &action, &higherTaskPriority);
		portYIELD_FROM_ISR(higherTaskPriority);
	}
}
