/*
 synchronize.c

  Created on: Jun 20, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "resources/connection-routines.h"

tHttpStatusCode SynchronizeCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);

  Routine_CallRoutine(
      &gConnectionRoutine, &SynchronizeRoutine, NULL);

  return HTTP_STATUS_OK;
}
