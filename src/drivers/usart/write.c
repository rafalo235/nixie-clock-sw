/*
 * write.c
 *
 *  Created on: Aug 20, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/usart/usart.h"
#include "stm32f103xb.h"
#include <string.h>

uint8_t gIsTransmissionStarted = 0;

uint8_t gTxBuffer[USART_TX_BUFFER_LENGTH];
uint16_t gTxEmpty = 0;
uint16_t gTxToSend = 0;
uint16_t gTxCurrentTransferLength;

static uint16_t GetEmptySpace(void);

void Usart_Write(uint8_t* data, uint16_t length)
{

  if (gIsTransmissionStarted)
    {
      struct TxData m;
      m.ptr = data;
      gTxCurrentTransferLength = m.length = length;
      xQueueSendToBack(txQueue, &m, portMAX_DELAY);
    }
  else
    {
      /* TODO synchronization */
      gIsTransmissionStarted = 1;
      gTxCurrentTransferLength = length;

      /* Clearing TC by writing 0 to it */
      USART1->SR = 0;

      Dma_StartTransfer(&gTxDma, data,
			(void*)&(USART1->DR),
			length);
    }
}

uint16_t Usart_WriteCopy(const uint8_t *data,
			 uint16_t length)
{
  /* Leave one byte to avoid overflow */
  uint16_t space = GetEmptySpace() - 1;

  length = space > length ? length : space;
  if (0 < length)
    {
      memcpy(&gTxBuffer[gTxEmpty], data, length);
      Usart_Write(&gTxBuffer[gTxEmpty], length);
      gTxEmpty += length;
      gTxEmpty %= USART_TX_BUFFER_LENGTH;
    }

  return length;
}

static uint16_t GetEmptySpace(void)
{
  uint16_t result = 0;
  uint16_t toSend = gTxToSend; //fixme synchronize

  if (gTxEmpty < toSend)
    {
      result = toSend - gTxEmpty;
    }
  else if (gTxEmpty > toSend)
    {
      result = USART_TX_BUFFER_LENGTH - gTxEmpty;
    }
  else
    {
      result = USART_TX_BUFFER_LENGTH;
    }
  return result;
}
