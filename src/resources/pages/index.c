/*
 index.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/html/header.h"
#include "resources/common.h"

tHttpStatusCode IndexCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/html");
  Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendCRLF(sm);

  /* Create and send content */
  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  Page_SendButton(conn, "Connect to AP", "loadConnect()");
  Page_SendButton(conn, "Set NTP Setting", "loadConnect()");
  Page_SendButton(conn, "Synchronize", "loadConnect()");
  Page_SendButton(conn, "Set Time", "loadConnect()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
