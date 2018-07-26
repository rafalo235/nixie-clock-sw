/*
 synchronize.c

  Created on: Jun 20, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "esp/esp.h"
#include "esp/esp_sntp.h"

tHttpStatusCode SynchronizeCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  esp_datetime_t dt;
  char buf[32];

  esp_sntp_gettime(&dt, 1u);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  Http_HelperSendHeader(sm);

  Http_HelperSendMessageBody(sm, "<html><head></head><body>");
  snprintf(buf, 32, "%u:%u:%u", dt.hours, dt.minutes, dt.seconds);
  Http_HelperSendMessageBody(sm, buf);
  Http_HelperSendMessageBody(sm, "</body><html>");

  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
