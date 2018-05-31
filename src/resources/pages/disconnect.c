/*
 disconnect.c

  Created on: Apr 18, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"

tHttpStatusCode DisconnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  gDisconnectFlag = 1;
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);

  Disconnect(&sEsp, Http_HelperGetContext(conn));

  return HTTP_STATUS_OK;
}
