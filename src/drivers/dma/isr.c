/*
 * isr.c
 *
 *  Created on: Jan 14, 2018
 *      Author: Rafał Olejniczak
 */

#include "drivers/dma/dma.h"

static void genericHandler(uint8_t channel);

/* Tx */
void DMA1_Channel4_IRQHandler(void)
{
  genericHandler(3);
}

/* Rx */
void DMA1_Channel5_IRQHandler(void)
{
  genericHandler(4);
}

static void genericHandler(uint8_t channel)
{
  uint32_t status = (DMA1->ISR >> (4 * channel)) & 0xF;

  if (gDmaChannels[channel])
    {
      if ((status & DMA_ISR_TCIF1) == DMA_ISR_TCIF1 &&
          gDmaChannels[channel]->transferCompleteCallback)
        {
          gDmaChannels[channel]->transferCompleteCallback();
        }

      if ((status & DMA_ISR_HTIF1) == DMA_ISR_HTIF1 &&
          gDmaChannels[channel]->halfTransferCallback)
        {
          gDmaChannels[channel]->halfTransferCallback();
        }

      if ((status & DMA_ISR_TEIF1) == DMA_ISR_TEIF1 &&
          gDmaChannels[channel]->transferErrorCallback)
        {
          gDmaChannels[channel]->transferErrorCallback();
        }
    }

  DMA1->IFCR = status << (4 * channel);
}
