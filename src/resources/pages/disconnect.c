/*
 disconnect.c

  Created on: Apr 18, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "resources/connection-routines.h"
#include "resources/routine.h"

tHttpStatusCode DisconnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  Routine_CallRoutine(
      &gConnectionRoutine, &DisconnectFromAccessPoint, NULL);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
