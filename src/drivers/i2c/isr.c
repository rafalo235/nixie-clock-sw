/*
 * isr.c
 *
 *  Created on: Aug 19, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/i2c/i2c.h"
#include "stm32f103xb.h"
#include "drivers/i2c/internal/buffer.h"
#include "utils/state-machine-framework.h"

typedef struct I2CContext
{
	tUtilsStateMachine sm;
	uint8_t index;
} tI2CContext;

typedef enum I2CAction
{
	I2C_START_CONDITION_GENERATED = UTILS_USER_SIGNAL,
	I2C_ADDRESS_SENT,
	I2C_BYTE_SENT
} tI2CAction;

static void OnStartState(void *c, tUtils_Signal sig);
static void OnAddressState(void *c, tUtils_Signal sig);
static void OnTransmissionState(void *c, tUtils_Signal sig);

static void NotifyCallingTask(tI2C_Result result);
static tI2CAction ConvertStatusToSignal(uint16_t status);

static tI2CContext sContext =
	{
			{ &OnStartState, &OnStartState },
			0
	};

void I2C1_EV_IRQHandler(void)
{
	uint16_t status = I2C1->SR1;

	Utils_ProcessStateMachine(&sContext,
			ConvertStatusToSignal(status));
}

void I2C1_ER_IRQHandler(void)
{

}

static void OnStartState(void *c, tUtils_Signal sig)
{
	tI2CContext *context = c;

	switch (sig)
	{
	case UTILS_ENTER_STATE :
	case UTILS_LEAVE_STATE :
		break;
	case I2C_START_CONDITION_GENERATED :
	{
		I2C1->DR = gI2CAddressBuffer;
		STATE_CHANGE(context, &OnAddressState);
		break;
	}
	default :
	{
		STATE_CHANGE(context, &OnStartState);
		NotifyCallingTask(I2C_ERROR);
		break;
	}
	}
}

static void OnAddressState(void *c, tUtils_Signal sig)
{
	tI2CContext *context = c;

	switch (sig)
	{
	case UTILS_ENTER_STATE :
	case UTILS_LEAVE_STATE :
		break;
	case I2C_ADDRESS_SENT :
	{
		volatile uint32_t dummy;
		dummy = I2C1->SR2;
		I2C1->DR = gI2CBuffer[0];
		context->index = 1;
		STATE_CHANGE(context, &OnTransmissionState);
		break;
	}
	default :
	{
		STATE_CHANGE(context, &OnStartState);
		NotifyCallingTask(I2C_ERROR);
		break;
	}
	}
}

static void OnTransmissionState(void *c, tUtils_Signal sig)
{
	tI2CContext *context = c;

	switch (sig)
	{
	case UTILS_ENTER_STATE :
	case UTILS_LEAVE_STATE :
		break;
	case I2C_BYTE_SENT :
	{
		if (context->index < gI2CBufferLength)
		{
			I2C1->DR = gI2CBuffer[context->index++];
		}
		else
		{
			uint8_t dummy = I2C1->DR;
			I2C1->CR1 |= I2C_CR1_STOP;
			STATE_CHANGE(context, &OnStartState);
			NotifyCallingTask(I2C_SUCCESS);
		}
		break;
	}
	default :
	{
		STATE_CHANGE(context, &OnStartState);
		NotifyCallingTask(I2C_ERROR);
		break;
	}
	}

}

static void NotifyCallingTask(tI2C_Result result)
{
	BaseType_t higherTaskPriority = pdFALSE;
	xTaskNotifyFromISR(gI2CCallingTask, result,
			eSetValueWithOverwrite, &higherTaskPriority);
	portYIELD_FROM_ISR(higherTaskPriority);

}

static tI2CAction ConvertStatusToSignal(uint16_t status)
{
	tI2CAction signal = 0xF;

	if ((I2C_SR1_SB & status) == I2C_SR1_SB)
	{
		signal = I2C_START_CONDITION_GENERATED;
	}
	else if ((I2C_SR1_ADDR & status) == I2C_SR1_ADDR)
	{
		signal = I2C_ADDRESS_SENT;
	}
	else if ((I2C_SR1_BTF & status) == I2C_SR1_BTF)
	{
		signal = I2C_BYTE_SENT;
	}

	return signal;
}
