/*
 common.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/common.h"
#include "resources/generated/html/button.h"
#include "resources/generated/html/passwordpopup.h"
#include "resources/generated/html/errorpopup.h"
#include "resources/generated/html/infoelement.h"
#include "resources/generated/html/input.h"
#include "resources/generated/html/header.h"
#include "uchttpserver.h"

static int sIsConnected = 0;

int Connection_IsConnected(void)
{
  return sIsConnected;
}

void Connection_SetConnected(int connected)
{
  sIsConnected = connected;
}

void Page_SendHtmlHeader(
    void * const conn, const char * title)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)title
  };
  Http_HelperSendParametered(sm, header_html, header_html_size, parameters);
}

void Page_SendInfoElement(
    void * const conn, const char * label, const char * value)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)label,
      (const void *)value
  };
  Http_HelperSendParametered(sm, infoelement_html, infoelement_html_size, parameters);
}

void Page_SendInput(
    void * const conn, const char * id,
    const char * label, const char * value)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)label,
      (const void *)id,
      (const void *)value
  };
  Http_HelperSendParametered(sm, input_html, input_html_size, parameters);
}

void Page_SendButton(
    void * const conn, const char * display, const char * callback)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)callback,
      (const void *)display
  };
  Http_HelperSendParametered(sm, button_html, button_html_size, parameters);
}

void Page_SendPasswordPopup(
    void * const conn, const char * modalid,
    const char * id, const char * onclick)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)modalid,
      (const void *)id,
      (const void *)onclick
  };
  Http_HelperSendParametered(
      sm, passwordpopup_html, passwordpopup_html_size, parameters);
}

void Page_SendErrorPopup(
    void * const conn, const char * modalid, const char * message)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)modalid,
      (const void *)message
  };
  Http_HelperSendParametered(
      sm, errorpopup_html, errorpopup_html_size, parameters);
}


void Json_OpenObject(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  Http_HelperSendMessageBody(conn, "{\n");
}

void Json_CloseObject(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  Http_HelperSendMessageBody(conn, "\n}");
}

void Json_OpenArray(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  Http_HelperSendMessageBody(conn, "[\n");
}

void Json_CloseArray(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  Http_HelperSendMessageBody(conn, "\n]");
}

void Json_Attribute(void * const conn, const char * name)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)name
  };
  Http_HelperSendMessageBodyParametered(conn, "\"%s\": ", parameters);
}

void Json_Value(void * const conn, const char * value)
{
  tuCHttpServerState * const sm = conn;
  const void * parameters[] = {
      (const void *)value
  };
  Http_HelperSendMessageBodyParametered(conn, "\"%s\"", parameters);
}

void Json_Separator(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  Http_HelperSendMessageBody(conn, ",");
}

void Disconnect(volatile int* esp, int* conn)
{
#if 0
  ESP_CONN_Close(esp, conn, 1);
  ReleaseServer(conn->Number);
#endif
}
