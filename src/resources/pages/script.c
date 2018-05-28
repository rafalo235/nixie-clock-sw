/*
 script.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/js/script.h"
#include "esp8266.h"

extern volatile ESP_t sEsp;

tHttpStatusCode ScriptCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/javascript");
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendHeader(sm);

  Http_HelperSend(sm, script_js, script_js_size);
  Http_HelperFlush(sm);

  ESP_CONN_Close(&sEsp, Http_HelperGetContext(conn), 1);

  return HTTP_STATUS_OK;
}


