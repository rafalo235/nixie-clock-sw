/*
 * resources.c
 *
 *  Created on: Mar 11, 2018
 *      Author: Rafał Olejniczak
 */

#include "port/uchttpserver/resources.h"

static tHttpStatusCode IndexCallback(void * const);

const tResourceEntry resources[] =
    {
	{ STRING_WITH_LENGTH("/index.html"), &IndexCallback }
    };

static tHttpStatusCode IndexCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  const char * helloWorld = "Hello world";
  const void * const * parameters = { (const void *)&helloWorld };

  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/html");
  Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendCRLF(sm);
  Http_HelperSendMessageBodyParametered(sm,
    "<html>"
    "<head>"
    "<meta http-equiv=\"Refresh\" content=\"1\" />"
    "</head>"
    "<body>"
    "<h1>Welcome to uCHttpServer!</h1>"
    "%s from uCHttpServer!"
    "</body>"
    "</html>", parameters);
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
