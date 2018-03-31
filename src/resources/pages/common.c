/*
 common.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/common.h"
#include "resources/generated/html/button.h"
#include "uchttpserver.h"

void Page_SendButton(
    void * const conn, const char * display, const char * callback)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)callback,
      (const void *)display
  };
  Http_HelperSendParametered(sm, button_html, button_html_size, parameters);
}
