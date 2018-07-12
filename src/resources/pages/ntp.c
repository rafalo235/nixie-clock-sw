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
#if 0
  tuCHttpServerState * const sm = conn;
  const char * ntp1 = Http_HelperGetParameter(sm, "ntp1");
  const char * ntp2 = Http_HelperGetParameter(sm, "ntp2");
  const char * ntp3 = Http_HelperGetParameter(sm, "ntp3");

  if (NULL != ntp1)
  {
    strncpy(SNTP_GetConfig()->Addr[0], ntp1, SNTP_ADDRESS_LEN);
  }
  if (NULL != ntp2)
  {
    strncpy(SNTP_GetConfig()->Addr[1], ntp2, SNTP_ADDRESS_LEN);
  }
  if (NULL != ntp3)
  {
    strncpy(SNTP_GetConfig()->Addr[2], ntp3, SNTP_ADDRESS_LEN);
  }
  /* todo timezone setup */
  SNTP_GetConfig()->Timezone = 0;

  Routine_CallRoutine(
      &gConnectionRoutine, &SetSNTPConfig, NULL);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSendHeader(sm);
  Http_HelperFlush(sm);
#endif

}

static void GetNtpCallback(void * const conn)
{
#if 0
  tuCHttpServerState * const sm = conn;

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  Http_HelperSendHeader(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  Page_SendInput(conn, "ntp1",
      "SNTP Server no 1", SNTP_GetConfig()->Addr[0]);
  Page_SendInput(conn, "ntp2",
      "SNTP Server no 2", SNTP_GetConfig()->Addr[1]);
  Page_SendInput(conn, "ntp3",
      "SNTP Server no 3", SNTP_GetConfig()->Addr[2]);

  /* TODO timezone combobox */

  Page_SendButton(conn, "Setup", "setupSNTP()");
  Page_SendButton(conn, "Back", "loadIndex()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);
#endif
}
