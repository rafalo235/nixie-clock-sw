/*
 index.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/html/header.h"
#include "resources/common.h"
#include "esp8266.h"

extern volatile ESP_t sEsp;

tHttpStatusCode IndexCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendHeader(sm);

  /* Create and send content */
  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  if (Connection_IsConnected())
    {
      Page_SendButton(conn, "Connection Status", "loadStatus()");
    }
  else
    {
      Page_SendButton(conn, "Connect to AP", "loadConnect()");
    }
  Page_SendButton(conn, "Set NTP Setting", "loadConnect()");
  Page_SendButton(conn, "Synchronize", "loadConnect()");
  Page_SendButton(conn, "Set Time", "loadConnect()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

  ESP_CONN_Close(&sEsp, Http_HelperGetContext(conn), 1);

  return HTTP_STATUS_OK;
}
