/*
 * transfer.c
 *
 *  Created on: Jan 14, 2018
 *      Author: Rafał Olejniczak
 */

#include "drivers/dma/dma.h"

void
Dma_StartTransfer(struct Dma *dma,
		  const void *from, void *to, uint16_t count)
{
  if ((dma->dmaChannelReg->CCR & DMA_CCR_DIR) == DMA_CCR_DIR)
    {
      dma->dmaChannelReg->CMAR = (uint32_t)from;
      dma->dmaChannelReg->CPAR = (uint32_t)to;
    }
  else
    {
      dma->dmaChannelReg->CPAR = (uint32_t)from;
      dma->dmaChannelReg->CMAR = (uint32_t)to;
    }
  dma->dmaChannelReg->CNDTR = count;
  Dma_Enable(dma);
}

uint16_t
Dma_GetRemainingBytes(struct Dma *dma)
{
  return dma->dmaChannelReg->CNDTR;
}
