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
#include "drivers/rtc/rtc.h"
#include "time/datetime.h"

void RTC_IRQHandler(void)
{
	const tControlAction action = CONTROL_ACTION_SECOND_TICK;
	BaseType_t higherTaskPriority = pdFALSE;
	tDatetime dt;
	uint32_t tmp;

	/* Clear pending interrupt */
	RTC->CRL &= ~RTC_CRL_SECF;

	Rtc_Read(&tmp);
	Rtc_GetDatetime(&dt);

	Datetime_Increment(&dt, tmp);
	Rtc_Write(0u);

	if (pdFALSE == xQueueIsQueueFullFromISR(gControlQueue))
	{
		xQueueSendToBackFromISR(
				gControlQueue, &action, &higherTaskPriority);
		portYIELD_FROM_ISR(higherTaskPriority);
	}
}
