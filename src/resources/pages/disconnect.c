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

  result = ESP_STA_Disconnect(&sEsp, 1);

  if (espOK == result)
    {
      Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
    }
  else
    {
      Http_HelperSendStatusLine(sm, HTTP_BAD_REQUEST);
    }
  Http_HelperSendCRLF(sm);

  return HTTP_STATUS_OK;
}
