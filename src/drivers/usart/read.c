/*
 * read.c
 *
 *  Created on: Jan 23, 2018
 *      Author: Rafał Olejniczak
 */

#include "drivers/usart/usart.h"
#include <string.h>
#include "stm32f103xb.h"

#define USART_RX_DMA_BUFFER_LENGTH  1024

struct Dma gRxDma =
{
  DMA1, DMA1_Channel5,
  NULL, NULL, NULL, 4
};

static uint16_t sReadPtr = 0;
static volatile uint8_t
gRxDmaBuffer[USART_RX_DMA_BUFFER_LENGTH];

#if 0
tUsartResult Usart_Read(uint8_t *data, uint16_t length, uint16_t *result)
{
  return USART_OK;
}
#endif


void Usart_InitializeRead(void)
{
  /* Receiver */
  Dma_Initialize(&gRxDma, DMA_CCR_MINC |
      DMA_CCR_CIRC);
  NVIC_SetPriority(DMA1_Channel5_IRQn, 5);
  Dma_StartTransfer(&gRxDma, (void*) &(USART1->DR),
      (void *)gRxDmaBuffer, USART_RX_DMA_BUFFER_LENGTH);

}

tUsartResult Usart_Read(volatile uint8_t **data, uint16_t *result)
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
#if 0
      *result = *result > length ? length : *result;
      memcpy(data, &gRxDmaBuffer[sReadPtr], *result);
#endif
      *data = &gRxDmaBuffer[sReadPtr];

      sReadPtr += *result;
      /* Zero pointer if reaches end */
      sReadPtr %= USART_RX_DMA_BUFFER_LENGTH;
    }

  return res;
}
