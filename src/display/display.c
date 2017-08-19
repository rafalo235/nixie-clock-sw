/*
 * display.c
 *
 *  Created on: Aug 12, 2017
 *      Author: Rafal Olejniczak
 */

#include "display/display.h"
#include "drivers/i2c/i2c.h"
#include "utils/coding.h"

void Display_Print(uint8_t number1, uint8_t number2)
{
#define SWAP_N_BITS(x,n)	(((x) << (n)) | ((x) >> (n)))

	number2 = Utils_BinToBcd(number2);
	number1 = Utils_BinToBcd(number1);

	number2 = SWAP_N_BITS(number2, 4);
	number1 = SWAP_N_BITS(number1, 4);

	I2C_WriteISR(0x70, &number2, 1);
	I2C_WriteISR(0x72, &number1, 1);

}
