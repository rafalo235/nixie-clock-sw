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

void Connection_Task(void *parameters)
{
  uint8_t buffer[16];
  uint16_t result;
  int i = 0;
  while (1)
  {

    Usart_Read(buffer, 16, &result);

    while (0 < result)
      {
	uint16_t coppied =
	    Usart_WriteCopy(buffer, result);
	result -= coppied;
      }

    if (((i++) % 1000) == 0)
      {
	Usart_Write("1234567890", 10);
      }

    vTaskDelay(pdMS_TO_TICKS(1));
  }

}
