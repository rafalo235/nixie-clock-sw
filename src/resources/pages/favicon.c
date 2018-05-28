/*
 favicon.c

  Created on: Mar 31, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "esp8266.h"

extern volatile ESP_t sEsp;

tHttpStatusCode FaviconCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_NOT_FOUND);
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);

  ESP_CONN_Close(&sEsp, Http_HelperGetContext(conn), 1);

  return HTTP_STATUS_OK;
}
