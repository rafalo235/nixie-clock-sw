/*
 * isr.c
 *
 *  Created on: Aug 8, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/gpio/gpio.h"
#include "control-task/control-task.h"
#include "stm32f103xb.h"

#define IS_RISING_EDGE(x) \
	((GPIOA->IDR & GPIO_IDR_IDR##x) == GPIO_IDR_IDR##x)
#define IS_FALLING_EDGE(x) \
	((GPIOA->IDR & GPIO_IDR_IDR##x) != GPIO_IDR_IDR##x)
#define IS_HIGH(x) \
	((GPIOA->IDR & GPIO_IDR_IDR##x) == GPIO_IDR_IDR##x)
#define IS_LOW(x) \
	((GPIOA->IDR & GPIO_IDR_IDR##x) != GPIO_IDR_IDR##x)

enum QuadratureState {
	ENCODER_IDLE,
	ENCODER_PENDING_DECREMENT,
	ENCODER_PENDING_INCREMENT
};

void EXTI1_IRQHandler(void)
{
	static enum QuadratureState sState = ENCODER_IDLE;

	EXTI->PR = EXTI_PR_PR1;

	if (ENCODER_IDLE == sState)
	{
		if (IS_FALLING_EDGE(1) && IS_HIGH(2))
		{
			sState = ENCODER_PENDING_DECREMENT;
		}
		else if (IS_FALLING_EDGE(1) && IS_LOW(2))
		{
			sState = ENCODER_PENDING_INCREMENT;
		}
	}
	else if (ENCODER_PENDING_DECREMENT == sState
			&& IS_RISING_EDGE(1) && IS_LOW(2))
	{
		tControlAction action = CONTROL_ACTION_DECREMENT;
		sState = ENCODER_IDLE;

		if (pdFALSE == xQueueIsQueueFullFromISR(gControlQueue))
		{
			BaseType_t higherTaskPriority = pdFALSE;
			xQueueSendToBackFromISR(
					gControlQueue, &action, &higherTaskPriority);
			portYIELD_FROM_ISR(higherTaskPriority);
		}
	}
	else if (ENCODER_PENDING_INCREMENT == sState
			&& IS_RISING_EDGE(1) && IS_HIGH(2))
	{
		tControlAction action = CONTROL_ACTION_INCREMENT;
		sState = ENCODER_IDLE;

		if (pdFALSE == xQueueIsQueueFullFromISR(gControlQueue))
		{
			BaseType_t higherTaskPriority = pdFALSE;
			xQueueSendToBackFromISR(
					gControlQueue, &action, &higherTaskPriority);
			portYIELD_FROM_ISR(higherTaskPriority);
		}
	}
	else
	{
		sState = ENCODER_IDLE;
	}
}


void EXTI2_IRQHandler(void)
{
	const tControlAction action = CONTROL_ACTION_INCREMENT;
	BaseType_t higherTaskPriority = pdFALSE;

	EXTI->PR = EXTI_PR_PR2;

	if ((GPIOA->IDR & GPIO_IDR_IDR1) == GPIO_IDR_IDR1 &&
			pdFALSE == xQueueIsQueueFullFromISR(gControlQueue))
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
