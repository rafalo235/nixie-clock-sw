/*
 * coding.c
 *
 *  Created on: Aug 7, 2017
 *      Author: rafal
 */

#include "utils/coding.h"

uint32_t Utils_BinToBcd(uint32_t value)
{
	uint32_t result;

	result = value % 10;
	value /= 10;

	result |= (value % 10) << 4;
	value /= 10;

	result |= (value % 10) << 8;
	value /= 10;

	result |= (value % 10) << 12;
	value /= 10;

	return result;
}
