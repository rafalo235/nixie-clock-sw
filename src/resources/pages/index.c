/*
 index.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/html/header.h"
#include "resources/common.h"
#include "resources/routine.h"
#include "resources/connection-routines.h"

tHttpStatusCode IndexCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  tRoutineStatus status;
  tConnectionRoutinesResults routineResult;
  int res;

#if 0
  status =
      Routine_GetRoutineResult(&gConnectionRoutine, &res);
  routineResult = (tConnectionRoutinesResults)res;
#endif

  /* Send header */
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendHeader(sm);

  /* Create and send content */
  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  if (Connection_IsConnected())
    {
      Page_SendButton(conn, "Connection Status", "loadStatus()");
    }
  else
    {
      Page_SendButton(conn, "Connect to AP", "loadConnect()");
    }
  Page_SendButton(conn, "Set NTP Setting", "loadNtp()");
  Page_SendButton(conn, "Synchronize", "loadConnect()");
  Page_SendButton(conn, "Set Time", "loadConnect()");

  if (ROUTINE_FINISHED == status)
  {
    Http_HelperSendMessageBody(sm,
        "<style>\n"
        ".modal {\n"
        "display: block;"
        "}\n"
        "</style>"
    );

    Page_SendErrorPopup(conn, "error-popup",
        ResolveResultMessage(routineResult));
  }

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
