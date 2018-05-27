/*
 disconnect.c

  Created on: Apr 18, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "esp8266.h"

/* todo header */
extern volatile ESP_t sEsp;

tHttpStatusCode DisconnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  ESP_Result_t result;

  if (espOK == (result = ESP_STA_Disconnect(&sEsp, 1)))
    {
      Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
      Connection_SetConnected(0);
    }
  else
    {
      Http_HelperSetResponseStatus(sm, HTTP_FORBIDDEN);
    }
  Http_HelperSendHeader(sm);

  return HTTP_STATUS_OK;
}
