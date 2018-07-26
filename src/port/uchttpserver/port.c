/*
 * port.c
 *
 *  Created on: Mar 11, 2018
 *      Author: Rafał Olejniczak
 */

#include "port/uchttpserver/port.h"
#include "uchttpserver.h"
#include "esp/esp.h"
#include "FreeRTOS.h"
#include "task.h"

extern volatile int sEsp;

unsigned int Http_SendPort(
    void * const  conn, const char * data, unsigned int length)
{
  size_t bw;

  while (1)
  {
    espr_t result = esp_netconn_write(
        Http_HelperGetContext((tuCHttpServerState*) conn), data, length);
    if (espOK != result)
    {
      vTaskDelay(pdMS_TO_TICKS(5));
    }
    else
    {
      result = esp_netconn_flush(Http_HelperGetContext((tuCHttpServerState*) conn));
      asm volatile ("nop");
      break;
    }
  }

  return length;
}
