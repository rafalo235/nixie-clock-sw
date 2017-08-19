/*
 * write.c
 *
 *  Created on: Aug 6, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/i2c/i2c.h"
#include "stm32f103xb.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/i2c/internal/buffer.h"

static tI2C_Result CheckArguments(uint8_t address, uint8_t data[], uint16_t length);
static tI2C_Result CreateStartCondition(void);
static tI2C_Result WriteAddress(uint8_t address);
static tI2C_Result WriteByte(uint8_t data);
static tI2C_Result CreateStopCondition(void);

tI2C_Result I2C_WriteISR(uint8_t address, uint8_t data[], uint16_t length)
{
	uint32_t result;

	/* TODO call synchronization with mutexes */

	gI2CAddressBuffer = address;
	gI2CBuffer = data;
	gI2CBufferLength = (uint8_t)length;
	gI2CCallingTask = xTaskGetCurrentTaskHandle();

	I2C1->CR1 |= I2C_CR1_START;

	/* Block till end of processing */
	xTaskNotifyWait(0xFFFFFFFFu, 0xFFFFFFFFu, &result, portMAX_DELAY);

	/* Wait for stop condition - fixme */
	while ((I2C1->SR2 & I2C_SR2_BUSY) == I2C_SR2_BUSY) { }

	/* TODO call synchronization with mutexes */

	return (tI2C_Result)result;
}

tI2C_Result I2C_Write(uint8_t address, uint8_t data[], uint16_t length)
{
	tI2C_Result result;

	if ((result = CheckArguments(address, data, length)) != I2C_SUCCESS)
	{
		return result;
	}

	if ((result = CreateStartCondition()) != I2C_SUCCESS)
	{
		return result;
	}

	if ((result = WriteAddress(address)) != I2C_SUCCESS)
	{
		return result;
	}

	while (length--)
	{
		if ((result = WriteByte(*data)) != I2C_SUCCESS)
		{
			return result;
		}
		++data;
	}

	if ((result = CreateStopCondition()) != I2C_SUCCESS)
	{
		return result;
	}

	return I2C_SUCCESS;
}

static tI2C_Result CheckArguments(uint8_t address, uint8_t data[], uint16_t length)
{
	tI2C_Result result = I2C_SUCCESS;
	if (address & 0x1)
	{
		result = I2C_LSB_SET_ON_WRITE;
	}
	else if (NULL == data)
	{
		result = I2C_BUFFER_IS_NULL;
	}
	else if (0 == length)
	{
		result = I2C_BUFFER_LENGTH_IS_ZERO;
	}
	return result;
}

static tI2C_Result CreateStartCondition(void)
{
	/* todo Check error */
	I2C1->CR1 |= I2C_CR1_START;
	while ((I2C1->SR1 & I2C_SR1_SB) != I2C_SR1_SB) { }
	return I2C_SUCCESS;
}

static tI2C_Result WriteAddress(uint8_t address)
{
	volatile uint32_t dummy;
	I2C1->DR = address;
	while ((I2C1->SR1 & I2C_SR1_ADDR) != I2C_SR1_ADDR) { }
	dummy = I2C1->SR2;
	return I2C_SUCCESS;
}

static tI2C_Result WriteByte(uint8_t data)
{
	I2C1->DR = data;
	while ((I2C1->SR1 & I2C_SR1_TXE) != I2C_SR1_TXE) { }

	return I2C_SUCCESS;
}

static tI2C_Result CreateStopCondition(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
	while ((I2C1->SR2 & I2C_SR2_BUSY) == I2C_SR2_BUSY) { }
	return I2C_SUCCESS;
}
