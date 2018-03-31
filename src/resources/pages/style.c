/*
 style.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/css/style.h"

tHttpStatusCode StyleCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/css");
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendCRLF(sm);

  Http_HelperSend(sm, style_css, style_css_size);

  return HTTP_STATUS_OK;
}
