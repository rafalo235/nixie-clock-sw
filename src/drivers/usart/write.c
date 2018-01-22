/*
 * write.c
 *
 *  Created on: Aug 20, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/usart/usart.h"
#include "stm32f103xb.h"

uint8_t gIsTransmissionStarted = 0;

void Usart_Write(uint8_t* data, uint16_t length)
{

  if (gIsTransmissionStarted)
    {
      struct TxData m;
      m.ptr = data;
      m.length = length;
      xQueueSendToBack(txQueue, &m, portMAX_DELAY);
    }
  else
    {
      /* TODO synchronization */
      gIsTransmissionStarted = 1;

      /* Clearing TC by writing 0 to it */
      USART1->SR = 0;

      Dma_StartTransfer(&gTxDma, data,
			(void*)&(USART1->DR),
			length);
    }
}
