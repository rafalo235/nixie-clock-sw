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

#define USART_RX_DMA_BUFFER_LENGTH (1024)
#define USART_TX_BUFFER_LENGTH	(1024)

typedef enum UsartResult
{
  USART_OK,
  USART_OVERRUN
} tUsartResult;

struct TxData
{
  const uint8_t *ptr;
  uint16_t length;
  uint8_t copy;
};

extern QueueHandle_t txQueue;
extern struct Dma gTxDma;
extern struct Dma gRxDma;
extern uint8_t gRxDmaBuffer[USART_RX_DMA_BUFFER_LENGTH];
extern uint8_t gTxBuffer[USART_TX_BUFFER_LENGTH];
extern uint16_t gTxEmpty;
extern uint16_t gTxToSend;
extern uint16_t gTxCurrentTransferLength;

void Usart_Initialize(void);
void Usart_Write(const uint8_t* data, uint16_t length);
uint16_t Usart_WriteCopy(const uint8_t *data,
			 uint16_t length);
tUsartResult Usart_Read(uint8_t *data, uint16_t length, uint16_t *result);

void Usart1_OnTransmissionComplete(void);
void Usart1_OnHalfTransmission(void);
void Usart1_OnTransmissionError(void);

#endif /* INC_DRIVERS_USART_USART_H_ */
