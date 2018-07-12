/*
 * port.c
 *
 *  Created on: Mar 11, 2018
 *      Author: Rafał Olejniczak
 */

#include "port/uchttpserver/port.h"
#include "uchttpserver.h"
#include "esp/esp.h"

extern volatile int sEsp;

unsigned int Http_SendPort(
    void * const  conn, const char * data, unsigned int length)
{
  uint32_t bw;

#if 0
  while (length)
    {
      ESP_Result_t result = ESP_CONN_Send(
          &sEsp, Http_HelperGetContext((tuCHttpServerState*)conn),
          data, length, &bw, 1);
      if (espOK != result)
      {
        break;
      }

      length -= bw;
      data += bw;
    }
#endif
  return length;
}
