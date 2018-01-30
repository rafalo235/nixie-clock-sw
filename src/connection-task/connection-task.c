/*
 * connection-task.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Rafal Olejniczak
 */

#include "connection-task/connection-task.h"
#include "esp8266.h"
#include "drivers/usart/usart.h"

int EspCallback(ESP_Event_t, ESP_EventParams_t *);

static volatile ESP_t sEsp;

void Connection_Task(void *parameters)
{
  ESP_AP_t aps[16];
  uint16_t res;

  if (espOK != ESP_Init(&sEsp, 115200, EspCallback))
    {
      asm volatile ("nop");
    }

  while (1)
    {
      ESP_ProcessCallbacks(&sEsp);
      vTaskDelay (pdMS_TO_TICKS (1));
      //ESP_STA_ListAccessPoints(&sEsp, aps, 16, &res, 1);
    }

}


void Receiver_Task(void *parameters)
{
#define RX_BUF_LEN	256
  uint8_t ch[RX_BUF_LEN];
  uint16_t res;
  while (1)
    {
      Usart_Read (ch, RX_BUF_LEN, &res);
      if (res)
	{
	  ESP_DataReceived (ch, res);
	}
      ESP_UpdateTime (&sEsp, 1);
      vTaskDelay (pdMS_TO_TICKS (1));
    }
}

void Update_Task(void *parameters)
{
  while (1)
    {
      ESP_Update(&sEsp);
    }
}


int EspCallback(ESP_Event_t ev, ESP_EventParams_t *param)
{
  (void)param;
  (void)ev;
  return 0;

}
