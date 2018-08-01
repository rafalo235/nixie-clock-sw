/*
 ntp.c

  Created on: Jun 2, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "resources/connection-routines.h"
#include "resources/routine.h"
#include "resources/generated/html/header.h"
#include "utils/utils.h"

static void PostNtpCallback(void * const conn);
static void GetNtpCallback(void * const conn);

tHttpStatusCode NtpCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  if (HTTP_POST == Http_HelperGetMethod(sm))
  {
    PostNtpCallback(conn);
  }
  else if (HTTP_GET == Http_HelperGetMethod(sm))
  {
    GetNtpCallback(conn);
  }

  return HTTP_STATUS_OK;
}

static void PostNtpCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  const char * ntp1 = Http_HelperGetParameter(sm, "ntp1");
  const char * ntp2 = Http_HelperGetParameter(sm, "ntp2");
  const char * ntp3 = Http_HelperGetParameter(sm, "ntp3");
  const char * tz = Http_HelperGetParameter(sm, "tz");
  espr_t result;

  if (NULL != ntp1)
  {
    strncpy(gConfigLocal.sntp[0], ntp1, SNTP_ADDRESS_LEN);
  }
  if (NULL != ntp2)
  {
    strncpy(gConfigLocal.sntp[1], ntp2, SNTP_ADDRESS_LEN);
  }
  if (NULL != ntp3)
  {
    strncpy(gConfigLocal.sntp[2], ntp3, SNTP_ADDRESS_LEN);
  }
  if (NULL != tz)
  {
    gConfigLocal.timezone = fast_atoi(tz);
  }

  result = esp_sntp_configure(1, gConfigLocal.timezone, gConfigLocal.sntp[0],
      gConfigLocal.sntp[1], gConfigLocal.sntp[2], 1u);

  Configuration_Set(&gConfigLocal);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);
}

static void GetNtpCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  char buf[4];

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  Http_HelperSendHeader(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  Page_SendInput(conn, "ntp1",
      "SNTP Server no 1", gConfigLocal.sntp[0]);
  Page_SendInput(conn, "ntp2",
      "SNTP Server no 2", gConfigLocal.sntp[1]);
  Page_SendInput(conn, "ntp3",
      "SNTP Server no 3", gConfigLocal.sntp[2]);
  snprintf(buf, 4, "%d", gConfigLocal.timezone);
  Page_SendInput(conn, "tz",
      "Time zone from -11 to 13", buf);

  Page_SendButton(conn, "Setup", "setupSNTP()");
  Page_SendButton(conn, "Back", "loadIndex()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);
}

