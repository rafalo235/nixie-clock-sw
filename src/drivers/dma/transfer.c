/*
 * transfer.c
 *
 *  Created on: Jan 14, 2018
 *      Author: Rafał Olejniczak
 */

#include "drivers/dma/dma.h"

void
Dma_StartTransfer(struct Dma *dma,
		  void *from, void *to, uint16_t count)
{
  dma->dmaChannelReg->CMAR = (uint32_t)from;
  dma->dmaChannelReg->CPAR = (uint32_t)to;
  dma->dmaChannelReg->CNDTR = count;
  Dma_Enable(dma);
}

