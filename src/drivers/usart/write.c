/*
 * write.c
 *
 *  Created on: Aug 20, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/usart/usart.h"
#include "stm32f103xb.h"

void Usart_Write(uint8_t* data, uint8_t length)
{
	while (length--)
	{
		USART1->DR = *data;
		++data;
		while ((USART1->SR & USART_SR_TXE) != USART_SR_TXE) { }
	}
}
