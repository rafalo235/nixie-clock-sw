/*
 connect.c

  Created on: Apr 5, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/html/header.h"
#include "resources/common.h"
#include "esp8266.h"
#include <string.h>


static void GetConnectCallback(void * const conn);
static void PostConnectCallback(void * const conn);

tHttpStatusCode ConnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  const char * apName = Http_HelperGetParameter(sm, "apn");
  const char * password = Http_HelperGetParameter(sm, "passwd");

  /* workaround due to limited parameter buffer */
  if (NULL != apName && NULL != password)
    {
      PostConnectCallback(conn);
    }
  else
    {
      GetConnectCallback(conn);
    }

  return HTTP_STATUS_OK;
}

static void GetConnectCallback(void * const conn)
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

  Http_HelperSendMessageBody(sm,
       "<div class=\"combo\">"
       "<select name=\"aps\" id=\"apslist\" size=\"8\"></select>"
       "</div>");

  Page_SendButton(conn, "Refresh", "refreshConnect()");
  Page_SendButton(conn, "Connect", "sendConnect()");
  Page_SendButton(conn, "Back", "loadIndex()");

  Page_SendPasswordPopup(
      conn, "passwordpopup", "apn_password",
      "connect()");
  Page_SendErrorPopup(conn, "error-popup", "Error occured");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

}

static void PostConnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  ESP_Result_t espResult;
  tHttpStatusCode status;
  const char * apName = Http_HelperGetParameter(sm, "apn");
  const char * password = Http_HelperGetParameter(sm, "passwd");

  if (NULL != apName && NULL != password)
  {
    status = HTTP_STATUS_OK;

    strncpy(gConnectApn, apName, 32);
    strncpy(gConnectPassword, password, 32);
    gConnectFlag = 1;

  } else
  {
    status = HTTP_STATUS_NOT_FOUND; /* fixme */
  }

  Http_HelperSetResponseStatus(sm, status);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  Http_HelperSendHeader(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSendMessageBody(sm, "</html>");

  Http_HelperFlush(sm);

  Disconnect(&sEsp, Http_HelperGetContext(conn));
}
