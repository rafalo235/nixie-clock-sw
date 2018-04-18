/*
 connect.c

  Created on: Apr 5, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/html/header.h"
#include "resources/common.h"
#include "esp8266.h"

extern volatile ESP_t sEsp;

static void GetConnectCallback(void * const conn);
static void PostConnectCallback(void * const conn);

tHttpStatusCode ConnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  tHttpMethod method = Http_HelperGetMethod(sm);

  if (HTTP_GET == method)
    {
      GetConnectCallback(conn);
    }
  else if (HTTP_POST == method)
    {
      PostConnectCallback(conn);
    }

  return HTTP_STATUS_OK;
}

static void GetConnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/html");
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendCRLF(sm);

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

      if (espOK == (espResult = ESP_STA_Connect(
          &sEsp, apName, password, NULL, 0, 1)))
        {
	  status = HTTP_STATUS_OK;
	  Connection_SetConnected(1);
        }
      else
	{
	  status = HTTP_STATUS_NOT_FOUND; /* fixme */
	  Connection_SetConnected(0);
	}
    }
  else
    {
      status = HTTP_STATUS_NOT_FOUND; /* fixme */
    }

  Http_HelperSendStatusLine(sm, status);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/html");
  Http_HelperSendCRLF(sm);

  if (NULL != apName && NULL != password)
    {
      Http_HelperSendMessageBody(sm, apName);
      Http_HelperSendMessageBody(sm, password);
    }
  Http_HelperFlush(sm);
}
