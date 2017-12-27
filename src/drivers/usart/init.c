/*
 * init.c
 *
 *  Created on: Aug 20, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/usart/usart.h"
#include "stm32f103xb.h"

static void InitializePins(void);
static void InitializeController(void);

void Usart_Initialize(void)
{
	InitializePins();
	InitializeController();

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

	/* move just before transmitting receiving */
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

}
