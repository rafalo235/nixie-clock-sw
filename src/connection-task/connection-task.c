/*
 * connection-task.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Rafal Olejniczak
 */

#include "connection-task/connection-task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/usart/usart.h"
#include "stm32f103xb.h"

void Connection_Task(void *parameters)
{
  uint8_t buffer[64];
  uint16_t result;
  int i = 0;

  /* Output 2 MHz push-pull */
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRH = (GPIOA->CRH & ~(GPIO_CRH_CNF11 | GPIO_CRH_MODE11)) |
      GPIO_CRH_MODE11_1;
  /* Set standby high */
  GPIOA->BSRR = GPIO_BSRR_BS11;

  while (1)
  {

    Usart_WriteCopy("AT\n\r", 4);
    vTaskDelay(pdMS_TO_TICKS(100));
    Usart_Read(buffer, 64, &result);
    vTaskDelay(pdMS_TO_TICKS(900));

  }

}
