/*
 common.c

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#include "resources/common.h"
#include "resources/generated/html/button.h"
#include "uchttpserver.h"

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
