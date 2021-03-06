/*
 * isr.c
 *
 *  Created on: Jan 21, 2018
 *      Author: Rafał Olejniczak
 */

#include "drivers/usart/usart.h"
#include "drivers/dma/dma.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "esp/esp_input.h"

// fixme to remove
uint8_t sReceiverEnabled = 0;

void USART1_IRQHandler(void)
{
  uint32_t status = USART1->SR;

  if ((status & USART_SR_TC) == USART_SR_TC)
  {
    struct TxData data;
    BaseType_t result;

    /* Clearing TC by writing 0 to it */
    USART1->SR = 0;

    gTxToSend += gTxCurrentTransferLength;
    gTxToSend %= USART_TX_BUFFER_LENGTH;

    result = xQueueReceiveFromISR(txQueue, &data, 0);
    if (pdPASS == result)
    {
      if (data.copy)
      {
        gTxCurrentTransferLength = data.length;
      } else
      {
        gTxCurrentTransferLength = 0;
      }
      Dma_StartTransfer(&gTxDma, data.ptr, (void*) &(USART1->DR), data.length);
    } else
    {
      /* TODO synchronization */
      gIsTransmissionStarted = 0;
      gTxCurrentTransferLength = 0;
    }
  }
#if 0
  else if (((status & USART_SR_RXNE) == USART_SR_RXNE)
      || ((status & USART_SR_ORE) == USART_SR_ORE))
  {
    uint8_t c = USART1->DR;
    if (sReceiverEnabled)
    {
      esp_input(&c, 1);
    }
  }
#endif
}

void Usart1_OnTransmissionComplete(void)
{
  Dma_Disable(&gTxDma);
}

void Usart1_OnHalfTransmission(void)
{

}

void Usart1_OnTransmissionError(void)
{

}
