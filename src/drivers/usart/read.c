/*
 * read.c
 *
 *  Created on: Jan 23, 2018
 *      Author: Rafał Olejniczak
 */

#include "drivers/usart/usart.h"
#include <string.h>

static uint16_t sReadPtr = 0;
uint8_t gRxDmaBuffer[USART_RX_DMA_BUFFER_LENGTH];

tUsartResult Usart_Read(uint8_t *data, uint16_t length, uint16_t *result)
{
  tUsartResult res = USART_OK;
  uint16_t pos = USART_RX_DMA_BUFFER_LENGTH - Dma_GetRemainingBytes(&gRxDma);

  if (sReadPtr < pos)
    {
      *result = pos - sReadPtr;
    }
  else if (sReadPtr > pos)
    {
      /* Buffer is wrapped */
      *result = USART_RX_DMA_BUFFER_LENGTH - sReadPtr;
    }
  else
    {
      *result = 0;
    }

  if (*result > 0)
    {
      *result = *result > length ? length : *result;
      memcpy(data, &gRxDmaBuffer[sReadPtr], *result);
      sReadPtr += *result;
      /* Zero pointer if reaches end */
      sReadPtr %= USART_RX_DMA_BUFFER_LENGTH;
    }

  return res;
}
