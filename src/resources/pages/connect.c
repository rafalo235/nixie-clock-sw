/*
 connect.c

  Created on: Apr 5, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/html/header.h"
#include "resources/common.h"
#include "resources/connection-routines.h"
#include "esp/esp.h"
#include <string.h>
#include "control-task/control-task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "connection-task/configuration.h"

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
  espr_t result;
  tHttpStatusCode status;
  const char * apName = Http_HelperGetParameter(sm, "apn");
  const char * password = Http_HelperGetParameter(sm, "passwd");
  uint8_t isConnected = esp_sta_is_joined();

  if (NULL != apName && NULL != password && (!isConnected))
  {
    status = HTTP_STATUS_OK;
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

  if (HTTP_STATUS_OK == status)
  {
    esp_netconn_flush(Http_HelperGetContext(sm));

    result = esp_sta_join(apName, password, NULL, 0, 1);

    if (espOK == result)
    {
      const tControlAction action = CONTROL_ACTION_SHOW_IP;
      strncpy(gConfigLocal.apn, apName, 32);
      strncpy(gConfigLocal.password, password, 32);
      xQueueSendToBack(gControlQueue, &action, portMAX_DELAY);

      Configuration_Set(&gConfigLocal);
    }
  }
}
