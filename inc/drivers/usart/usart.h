/*
 * usart.h
 *
 *  Created on: Aug 20, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_DRIVERS_USART_USART_H_
#define INC_DRIVERS_USART_USART_H_

#include <stdint.h>

void Usart_Initialize(void);
void Usart_Write(uint8_t* data, uint8_t length);

#endif /* INC_DRIVERS_USART_USART_H_ */
