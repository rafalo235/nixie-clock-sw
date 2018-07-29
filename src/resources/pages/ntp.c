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

char sSNTPAddress[3][SNTP_ADDRESS_LEN];
int8_t sSNTPTimeZone = 0;

static void PostNtpCallback(void * const conn);
static void GetNtpCallback(void * const conn);

int fast_atoi( const char * str )
{
    int val = 0;
    int multiplier = 1;

    if (*str == '-')
    {
      multiplier = -1;
    }

    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val * multiplier;
}

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
    strncpy(sSNTPAddress[0], ntp1, SNTP_ADDRESS_LEN);
  }
  if (NULL != ntp2)
  {
    strncpy(sSNTPAddress[1], ntp2, SNTP_ADDRESS_LEN);
  }
  if (NULL != ntp3)
  {
    strncpy(sSNTPAddress[2], ntp3, SNTP_ADDRESS_LEN);
  }
  if (NULL != tz)
  {
    sSNTPTimeZone = fast_atoi(tz);
  }

  result = esp_sntp_configure(1, sSNTPTimeZone, sSNTPAddress[0],
      sSNTPAddress[1], sSNTPAddress[2], 1u);

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
      "SNTP Server no 1", sSNTPAddress[0]);
  Page_SendInput(conn, "ntp2",
      "SNTP Server no 2", sSNTPAddress[1]);
  Page_SendInput(conn, "ntp3",
      "SNTP Server no 3", sSNTPAddress[2]);
  snprintf(buf, 4, "%d", sSNTPTimeZone);
  Page_SendInput(conn, "tz",
      "Time zone from -11 to 13", buf);

  Page_SendButton(conn, "Setup", "setupSNTP()");
  Page_SendButton(conn, "Back", "loadIndex()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);
}

void SNTP_Initialize(void)
{
  /* todo read eeprom */
  sSNTPAddress[0][0] = (char)0u;
  sSNTPAddress[1][0] = (char)0u;
  sSNTPAddress[2][0] = (char)0u;
}

