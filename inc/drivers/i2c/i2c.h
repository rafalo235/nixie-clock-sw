/*
 * i2c.h
 *
 *  Created on: Aug 6, 2017
 *      Author: rafal
 */

#ifndef INC_DRIVERS_I2C_I2C_H_
#define INC_DRIVERS_I2C_I2C_H_

#include <stdint.h>

typedef enum I2C_Result
{
	I2C_SUCCESS,
	I2C_LSB_SET_ON_WRITE,
	I2C_BUFFER_IS_NULL,
	I2C_BUFFER_LENGTH_IS_ZERO,
	I2C_ERROR
} tI2C_Result;

void I2C_Initialize(void);
tI2C_Result I2C_Write(uint8_t address, uint8_t data[], uint16_t length);
tI2C_Result I2C_WriteISR(uint8_t address, uint8_t data[], uint16_t length);

#endif /* INC_DRIVERS_I2C_I2C_H_ */
