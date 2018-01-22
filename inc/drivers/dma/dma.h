/*
 * dma.h
 *
 *  Created on: Dec 28, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_DRIVERS_DMA_DMA_H_
#define INC_DRIVERS_DMA_DMA_H_

#include "stm32f103xb.h"

#define DMA_CHANNELS_AMOUNT (12)

typedef void (*tDmaIsrCallback)(void);

struct Dma
{
  DMA_TypeDef *dmaControllerReg;
  DMA_Channel_TypeDef *dmaChannelReg;
  tDmaIsrCallback halfTransferCallback;
  tDmaIsrCallback transferCompleteCallback;
  tDmaIsrCallback transferErrorCallback;
  uint8_t channel;
};

extern struct Dma *gDmaChannels[DMA_CHANNELS_AMOUNT];
extern uint8_t gIsTransmissionStarted;

void
Dma_Initialize(struct Dma *dma,
	       uint32_t ccr);

void
Dma_Enable(struct Dma *dma);

void
Dma_Disable(struct Dma *dma);

void
Dma_StartTransfer(struct Dma *dma,
		  void *from, void *to, uint16_t count);

#endif /* INC_DRIVERS_DMA_DMA_H_ */
