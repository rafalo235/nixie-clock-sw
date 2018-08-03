/*
 favicon.c

  Created on: Mar 31, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/img/favicon.h"
#include "resources/common.h"

tHttpStatusCode FaviconCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "image/x-icon");
  Http_HelperSendHeader(sm);

  Http_HelperSend(sm, favicon_ico, favicon_ico_size);
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
