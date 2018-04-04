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
static void QueueForTransfer(const uint8_t *data,
			     uint16_t length,
			     uint8_t copy);

void Usart_Write(const uint8_t* data, uint16_t length)
{
  QueueForTransfer(data, length, 0);
}

uint16_t Usart_WriteCopy(const uint8_t *data,
			 uint16_t length)
{
  /* Leave one byte to avoid overflow */
  uint16_t space = GetEmptySpace();

  length = space > length ? length : space;
  if (0 < length)
    {
      memcpy(&gTxBuffer[gTxEmpty], data, length);
      QueueForTransfer(&gTxBuffer[gTxEmpty], length, 1);
      gTxEmpty += length;
      gTxEmpty %= USART_TX_BUFFER_LENGTH;
    }

  return length;
}

static uint16_t GetEmptySpace(void)
{
  uint16_t result = 0;
  uint16_t toSend;
  uint8_t started;

  __disable_irq();
  started = gIsTransmissionStarted;
  toSend = gTxToSend;
  __enable_irq();

  if (gTxEmpty < toSend)
    {
      result = toSend - gTxEmpty - 1;
    }
  else if (gTxEmpty > toSend)
    {
      result = USART_TX_BUFFER_LENGTH - gTxEmpty;
    }
  else /* if equal */
    {
      if (started)
	{
	  result = 0;
	}
      else
	{
	  /* Everything is sent */
	  result = USART_TX_BUFFER_LENGTH - gTxEmpty;
	}
    }
  return result;
}

static void QueueForTransfer(const uint8_t *data,
			     uint16_t length,
			     uint8_t copy)
{
  __disable_irq();

  if (gIsTransmissionStarted)
    {
      struct TxData m;
      m.ptr = data;
      m.length = length;
      m.copy = copy;

      __enable_irq();

      xQueueSendToBack(txQueue, &m, portMAX_DELAY);
    }
  else
    {
      /* TODO synchronization */
      gIsTransmissionStarted = 1;
      if (copy)
	{
	  gTxCurrentTransferLength = length;
	}
      else
	{
	  gTxCurrentTransferLength = 0;
	}

      /* Clearing TC by writing 0 to it */
      USART1->SR = 0;

      Dma_StartTransfer(&gTxDma, data,
			(void*)&(USART1->DR),
			length);

      __enable_irq();
    }

}
