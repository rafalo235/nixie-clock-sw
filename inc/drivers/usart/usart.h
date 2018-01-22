/*
 * usart.h
 *
 *  Created on: Aug 20, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_DRIVERS_USART_USART_H_
#define INC_DRIVERS_USART_USART_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "drivers/dma/dma.h"

struct TxData
{
  uint8_t *ptr;
  uint16_t length;
};

extern QueueHandle_t txQueue;
extern struct Dma gTxDma;

void Usart_Initialize(void);
void Usart_Write(uint8_t* data, uint16_t length);

void Usart1_OnTransmissionComplete(void);
void Usart1_OnHalfTransmission(void);
void Usart1_OnTransmissionError(void);

#endif /* INC_DRIVERS_USART_USART_H_ */
