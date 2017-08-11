/*
 * isr.c
 *
 *  Created on: Aug 8, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/gpio/gpio.h"
#include "control-task/control-task.h"
#include "stm32f103xb.h"

void EXTI1_IRQHandler(void)
{
	const tControlAction action = CONTROL_ACTION_DECREMENT;
	BaseType_t higherTaskPriority = pdFALSE;

	EXTI->PR = EXTI_PR_PR1;

	if (pdFALSE == xQueueIsQueueFullFromISR(gControlQueue) &&
			(GPIOA->IDR & GPIO_IDR_IDR2) != GPIO_IDR_IDR2)
	{
		xQueueSendToBackFromISR(
				gControlQueue, &action, &higherTaskPriority);
		portYIELD_FROM_ISR(higherTaskPriority);
	}
}


void EXTI2_IRQHandler(void)
{
	const tControlAction action = CONTROL_ACTION_INCREMENT;
	BaseType_t higherTaskPriority = pdFALSE;

	EXTI->PR = EXTI_PR_PR2;

	if (pdFALSE == xQueueIsQueueFullFromISR(gControlQueue) &&
			(GPIOA->IDR & GPIO_IDR_IDR1) != GPIO_IDR_IDR1)
	{
		xQueueSendToBackFromISR(
				gControlQueue, &action, &higherTaskPriority);
		portYIELD_FROM_ISR(higherTaskPriority);
	}
}


void EXTI3_IRQHandler(void)
{
	const tControlAction action = CONTROL_ACTION_PRESS;
	BaseType_t higherTaskPriority = pdFALSE;

	EXTI->PR = EXTI_PR_PR3;

	if (pdFALSE == xQueueIsQueueFullFromISR(gControlQueue))
	{
		xQueueSendToBackFromISR(
				gControlQueue, &action, &higherTaskPriority);
		portYIELD_FROM_ISR(higherTaskPriority);
	}
}
