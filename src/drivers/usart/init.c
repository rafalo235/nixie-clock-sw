/*
 * init.c
 *
 *  Created on: Aug 20, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/usart/usart.h"
#include "drivers/dma/dma.h"
#include "stm32f103xb.h"

QueueHandle_t txQueue;

struct Dma gTxDma =
    {
	DMA1, DMA1_Channel4,
	Usart1_OnHalfTransmission,
	Usart1_OnTransmissionComplete,
	Usart1_OnTransmissionError,
	3
    };

static void InitializePins(void);
static void InitializeController(void);
static void InitializeTxQueue(void);
static void InitializeDma(void);

void Usart_Initialize(void)
{
  InitializeTxQueue();
  InitializePins();
  InitializeDma();
  InitializeController();
  NVIC_EnableIRQ(USART1_IRQn);
}

static void InitializePins(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

  GPIOA->CRH = (GPIOA->CRH & ~(GPIO_CRH_MODE9 | GPIO_CRH_MODE10 |
		GPIO_CRH_CNF9 | GPIO_CRH_CNF10)) |
		GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_1 | /* Output 2MHz AF PP */
		GPIO_CRH_CNF10_0; /* Rx Input floating - todo pull-up (ODR config) */
}

static void InitializeController(void)
{
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  USART1->CR1 = USART_CR1_UE;
  /* Default - 1 stop bit
   * 19,5 BRR */
  USART1->BRR = 0x138;

  USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;

  /* move just before transmitting receiving */
  USART1->CR1 |= USART_CR1_TE | USART_CR1_RE
      | USART_CR1_TCIE; /*| USART_CR1_RXNEIE ;*/

  USART1->SR = 0;

  NVIC_SetPriority(USART1_IRQn, 5);

}

static void InitializeTxQueue(void)
{
  txQueue = xQueueCreate(16, sizeof(struct TxData));
}

static void InitializeDma(void)
{
  /* Transmitter */
  Dma_Initialize(&gTxDma, DMA_CCR_MINC |
		 DMA_CCR_DIR | DMA_CCR_TCIE |
		 DMA_CCR_TEIE);
  NVIC_SetPriority(DMA1_Channel4_IRQn, 5);
  NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}
