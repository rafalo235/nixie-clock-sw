/*
 connect.c

  Created on: Apr 5, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/generated/html/header.h"
#include "resources/common.h"

tHttpStatusCode ConnectCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;

  /* Send header */
  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/html");
  //Http_HelperSendHeaderLine(sm, "Connection", "close");
  Http_HelperSendCRLF(sm);

  /* Create and send content */
  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  Http_HelperSendMessageBody(sm,
       "<div class=\"combo\">"
       "<select name=\"aps\" id=\"apslist\" size=\"8\"></select>"
       "</div>");

  Page_SendButton(conn, "Refresh", "refreshConnect()");
  Page_SendButton(conn, "Connect", "sendConnect()");
  Page_SendButton(conn, "Back", "loadIndex()");

  Http_HelperSendMessageBody(sm,
       "<div id=\"passwordpopup\" class=\"modal\">"
	 "<div class=\"modal-content\">"
	   "<p>Enter password</p>"
	  "<input type=\"password\" id=\"apn_password\" />"
	  "<div class='button popup-button' onclick='displayError(\"Not implemented yet\")'><p>OK</p></div>"
	 "</div>"
       "</div>"
       "<div id=\"error-popup\" class=\"modal\">"
	 "<div class=\"modal-content\">"
	   "<p id=\"error-message\">Error occured</p>"
	 "</div>"
       "</div>");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
