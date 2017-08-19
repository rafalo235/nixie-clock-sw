/*
 * init.c
 *
 *  Created on: Aug 6, 2017
 *      Author: Rafal Olejniczak
 */

#include "drivers/i2c/i2c.h"
#include "stm32f103xb.h"

static void InitializePins(void);
static void InitializeController(void);
static void InitializeInterrupts(void);

void I2C_Initialize(void)
{
	InitializePins();
	InitializeController();
	InitializeInterrupts();
}

static void InitializePins(void)
{
#define SCL_OUTPUT_50MHZ	(GPIO_CRL_MODE6)
#define SCL_OPEN_DRAIN	(GPIO_CRL_CNF6)
#define SDA_OUTPUT_50MHZ	(GPIO_CRL_MODE7)
#define SDA_OPEN_DRAIN	(GPIO_CRL_CNF7)

	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	GPIOB->CRL = (GPIOB->CRL &
                 ~(GPIO_CRL_CNF6 | GPIO_CRL_CNF7 |
                  GPIO_CRL_MODE6 | GPIO_CRL_MODE7)) |
         SCL_OUTPUT_50MHZ | SCL_OPEN_DRAIN |
         SDA_OUTPUT_50MHZ | SDA_OPEN_DRAIN;
}

static void InitializeController(void)
{
#define OWN_ADDRESS		0xFE
	/* 8 MHz / 40 = 200 000 Hz (Thigh = Tlow)
	 * 200 000 Hz / 2 = 100 000 Hz = T */
#define FREQUENCY_PRESCALER	40
	/* Maximum allowed rise time for Slow mode = 1000ns
	 * Tclk = 125 ns
	 * 1000ns / 125 ns = 8
	 * TRISE = 8 + 1 */
#define TRISE_VALUE	9
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	I2C1->CR1 = 0;

	/* APB2 8 MHz clock */
	I2C1->CR2 = I2C_CR2_FREQ_3 | I2C_CR2_ITEVTEN;
	I2C1->OAR1 = OWN_ADDRESS;
	I2C1->CCR = FREQUENCY_PRESCALER;
	I2C1->TRISE = TRISE_VALUE;

	I2C1->CR1 = I2C_CR1_PE;
}

static void InitializeInterrupts(void)
{
	NVIC_SetPriority(I2C1_EV_IRQn, 5);
	NVIC_EnableIRQ(I2C1_EV_IRQn);
}
