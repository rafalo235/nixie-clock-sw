/*
 * init.c
 *
 *  Created on: Dec 28, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/dma/dma.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

struct Dma *gDmaChannels[DMA_CHANNELS_AMOUNT] = { 0 };

static void InitializeClock(void);

void
Dma_Initialize(struct Dma *dma,
	       uint32_t ccr)
{
  configASSERT(dma != NULL);
  configASSERT(dma->channel < 12);

  InitializeClock();
  gDmaChannels[dma->channel] = dma;
  dma->dmaChannelReg->CCR = ccr;
}

void
Dma_Enable(struct Dma *dma)
{
  dma->dmaChannelReg->CCR |= DMA_CCR_EN;
}

void
Dma_Disable(struct Dma *dma)
{
  dma->dmaChannelReg->CCR &= ~DMA_CCR_EN;
}

static void InitializeClock(void)
{
  /* todo reference to clock handler */
  if ((RCC->AHBENR & RCC_AHBENR_DMA1EN)
      != RCC_AHBENR_DMA1EN)
    {
      RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    }
}
