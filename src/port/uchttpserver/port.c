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

  while (length)
  {
    espr_t result = esp_conn_send(
        Http_HelperGetContext((tuCHttpServerState*)conn),
        data, length, &bw, 1u);
    if (espOK != result)
    {
      vTaskDelay(pdMS_TO_TICKS(5));
    }
    length -= bw;
    data += bw;
  }

  return length;
}
