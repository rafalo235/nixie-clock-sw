/*
 status.c

 MIT License

  Created on: Apr 18, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "resources/generated/html/header.h"
#include "esp8266.h"

extern volatile ESP_t sEsp;

tHttpStatusCode StatusCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  char buf[5];
  ESP_ConnectedAP_t connected;
  ESP_Result_t result = ESP_STA_GetConnected(&sEsp, &connected, 1);


  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/html");
  Http_HelperSendCRLF(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  Http_HelperSendMessageBody(sm, "<div class=\"info\">");

  Http_HelperSendMessageBody(sm, "<p>");
  Http_HelperSendMessageBody(sm, "Access point: ");
  Http_HelperSendMessageBody(sm, connected.SSID);
  Http_HelperSendMessageBody(sm, "</p>");

  snprintf(buf, 5, "%d", connected.RSSI);
  Http_HelperSendMessageBody(sm, "<p>");
  Http_HelperSendMessageBody(sm, "Signal strength: ");
  Http_HelperSendMessageBody(sm, buf);
  Http_HelperSendMessageBody(sm, "</p>");

  Http_HelperSendMessageBody(sm, "</div>");

  Page_SendButton(conn, "Disconnect", "loadDisconnect()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
