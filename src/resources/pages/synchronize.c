/*
 synchronize.c

  Created on: Jun 20, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "esp/esp.h"
#include "esp/esp_sntp.h"
#include "drivers/rtc/rtc.h"
#include "resources/generated/html/header.h"

tHttpStatusCode SynchronizeCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  esp_datetime_t dt;
  char buf[24];

  esp_sntp_gettime(&dt, 1u);

  dt.date--;
  dt.month--;
  Rtc_SetDatetime(&dt);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  Http_HelperSendHeader(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  Http_HelperSendMessageBody(sm, "<div class=\"info\">");
  snprintf(buf, 24, "%04u.%02u.%02u %02u:%02u:%02u",
      dt.year, dt.month + 1, dt.date + 1,
      dt.hours, dt.minutes, dt.seconds);
  Page_SendInfoElement(conn, "Time set to", buf);
  Http_HelperSendMessageBody(sm, "</div>");

  Page_SendButton(conn, "Back", "loadIndex()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");

  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
