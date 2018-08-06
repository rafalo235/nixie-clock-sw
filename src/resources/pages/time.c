/*
 time.c

  Created on: Jul 29, 2018
      Author: Rafał Olejniczak
 */
#include "resources/pages.h"
#include "resources/common.h"
#include "time/datetime.h"
#include "drivers/rtc/rtc.h"
#include "utils/utils.h"
#include <stdlib.h>

static void PostTimeCallback(void * const conn);
static void GetTimeCallback(void * const conn);

tHttpStatusCode TimeCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  if (HTTP_POST == Http_HelperGetMethod(sm))
  {
    PostTimeCallback(conn);
  }
  else if (HTTP_GET == Http_HelperGetMethod(sm))
  {
    GetTimeCallback(conn);
  }

  return HTTP_STATUS_OK;
}

static void PostTimeCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  const char * tmp = NULL;
  tDatetime dt;

  tmp = Http_HelperGetParameter(sm, "ye");
  dt.year = fast_atoi(tmp);

  tmp = Http_HelperGetParameter(sm, "mo");
  dt.month = fast_atoi(tmp) - 1;

  tmp = Http_HelperGetParameter(sm, "da");
  dt.date = fast_atoi(tmp) - 1;

  tmp = Http_HelperGetParameter(sm, "ho");
  dt.hours = fast_atoi(tmp);

  tmp = Http_HelperGetParameter(sm, "mi");
  dt.minutes = fast_atoi(tmp);

  tmp = Http_HelperGetParameter(sm, "se");
  dt.seconds = fast_atoi(tmp);

  Rtc_SetDatetime(&dt);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);
}

static void GetTimeCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  char buf[5];
  tDatetime dt;

  Rtc_GetDatetime(&dt);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  Http_HelperSendHeader(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Page_SendHtmlHeader(conn, "Manual Time Setup");
  Http_HelperSendMessageBody(sm, "<body>");

  snprintf(buf, 5, "%04u", dt.year);
  Page_SendInput(conn, "ye", "Year", buf);
  snprintf(buf, 5, "%02u", dt.month + 1);
  Page_SendInput(conn, "mo", "Month", buf);
  snprintf(buf, 5, "%02u", dt.date + 1);
  Page_SendInput(conn, "da", "Day of month", buf);
  snprintf(buf, 5, "%02u", dt.hours);
  Page_SendInput(conn, "ho", "Hour", buf);
  snprintf(buf, 5, "%02u", dt.minutes);
  Page_SendInput(conn, "mi", "Minute", buf);
  snprintf(buf, 5, "%02u", dt.seconds);
  Page_SendInput(conn, "se", "Second", buf);

  Page_SendButton(conn, "Setup", "setupTime()");
  Page_SendButton(conn, "Back", "loadIndex()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);
}

