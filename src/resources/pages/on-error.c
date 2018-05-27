/*
 on-error.c

  Created on: May 27, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"

void OnError(
    void *const conn,
    const tErrorInfo *errorInfo)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSetResponseStatus(sm, errorInfo->status);
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);
}
