/*
 * connection-task.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Rafal Olejniczak
 */

#include "connection-task/connection-task.h"
#include "esp8266.h"

int EspCallback(ESP_Event_t, ESP_EventParams_t *);

void Connection_Task(void *parameters)
{
  static volatile ESP_t sEsp;
  ESP_AP_t aps[16];
  uint16_t res;

  ESP_Init(&sEsp, 115200, EspCallback);
  ESP_STA_ListAccessPoints(&sEsp, aps, 16, &res, 1);

  while (1)
  {


  }

}



int EspCallback(ESP_Event_t ev, ESP_EventParams_t *param)
{
  (void)param;
  (void)ev;
  return 1;

}
