/*
 * buffer.h
 *
 *  Created on: Aug 19, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_DRIVERS_I2C_INTERNAL_BUFFER_H_
#define INC_DRIVERS_I2C_INTERNAL_BUFFER_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

extern uint8_t gI2CAddressBuffer;
extern uint8_t *gI2CBuffer;
extern uint8_t gI2CBufferLength;
extern TaskHandle_t gI2CCallingTask;

#endif /* INC_DRIVERS_I2C_INTERNAL_BUFFER_H_ */
