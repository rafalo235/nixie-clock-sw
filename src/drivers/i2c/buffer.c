/*
 * buffer.c
 *
 *  Created on: Aug 19, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/i2c/internal/buffer.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t gI2CAddressBuffer = 0;
uint8_t* gI2CBuffer;
uint8_t gI2CBufferLength = 0;
TaskHandle_t gI2CCallingTask;
