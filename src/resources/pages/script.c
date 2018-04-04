/*
 script.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/js/script.h"

tHttpStatusCode ScriptCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/javascript");
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendCRLF(sm);

  Http_HelperSend(sm, script_js, script_js_size);
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}


