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
  while (1)
  {

    Usart_Read(buffer, 16, &result);

    if (result > 0)
      {
	Usart_Write(buffer, result);
      }
    vTaskDelay(pdMS_TO_TICKS(100));
  }

}
