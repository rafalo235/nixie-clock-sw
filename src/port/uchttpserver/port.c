/*
 * port.c
 *
 *  Created on: Mar 11, 2018
 *      Author: Rafał Olejniczak
 */

#include "port/uchttpserver/port.h"
#include "uchttpserver.h"
#include "esp8266.h"

extern volatile ESP_t sEsp;

unsigned int Http_SendPort(
    void * const  conn, const char * data, unsigned int length)
{
  uint32_t bw;
  /* todo check */
  ESP_CONN_Send(
      &sEsp, Http_HelperGetContext((tuCHttpServerState*)conn),
      data, length, &bw, 0);
  return length;
}
