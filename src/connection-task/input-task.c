/*
 input-task.c

  Created on: Jul 23, 2018
      Author: Rafał Olejniczak
 */

#include "connection-task/connection-task.h"
#include "esp/esp.h"
#include "esp/esp_input.h"
#include "drivers/usart/usart.h"
#include "FreeRTOS.h"
#include "task.h"

void Input_Task(void *parameters)
{
  TickType_t xLastWakeTime;

  (void)parameters;

  xLastWakeTime = xTaskGetTickCount();

  while (1)
  {
    uint16_t len;
    volatile uint8_t *buffer;

    do
    {
      Usart_Read(&buffer, &len);

      if (0u != len)
      {
        /* Optimizations must be turned off only in Input_Task scope */
        esp_input_process((uint8_t*)buffer, len);
      }
    }
    while (0u != len);

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(35));
  }
}
