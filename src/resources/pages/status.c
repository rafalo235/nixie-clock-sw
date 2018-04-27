/*
 status.c

 MIT License

  Created on: Apr 18, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "resources/generated/html/header.h"
#include "esp8266.h"

#define LOCAL_BUFFER_SIZE	20

extern volatile ESP_t sEsp;

tHttpStatusCode StatusCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  char buf[LOCAL_BUFFER_SIZE];
  ESP_ConnectedAP_t connected;
  ESP_Result_t result = ESP_STA_GetConnected(&sEsp, &connected, 1);


  Http_HelperSendStatusLine(sm, HTTP_STATUS_OK);
  Http_HelperSendHeaderLine(sm, "Content-Type", "text/html");
  Http_HelperSendCRLF(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Http_HelperSend(sm, header_html, header_html_size);
  Http_HelperSendMessageBody(sm, "<body>");

  Http_HelperSendMessageBody(sm, "<div class=\"info\">");

  /* Info elements */
  if (espOK == result)
    {
      Page_SendInfoElement(sm, "Access point", connected.SSID);

      snprintf(buf, LOCAL_BUFFER_SIZE, "%d", connected.RSSI);
      Page_SendInfoElement(sm, "Signal strength", buf);

      snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u",
	       sEsp.STAIP[0], sEsp.STAIP[1],
	       sEsp.STAIP[2], sEsp.STAIP[3]);
      Page_SendInfoElement(sm, "Station IP", buf);
      snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u",
	       sEsp.STAGateway[0], sEsp.STAGateway[1],
	       sEsp.STAGateway[2], sEsp.STAGateway[3]);
      Page_SendInfoElement(sm, "Station Gateway", buf);
      snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u",
	       sEsp.STANetmask[0], sEsp.STANetmask[1],
	       sEsp.STANetmask[2], sEsp.STANetmask[3]);
      Page_SendInfoElement(sm, "Station Netmask", buf);
      snprintf(buf, LOCAL_BUFFER_SIZE, "%02X:%02X:%02X:%02X:%02X:%02X",
	       sEsp.STAMAC[0], sEsp.STAMAC[1],
	       sEsp.STAMAC[2], sEsp.STAMAC[3],
	       sEsp.STAMAC[4], sEsp.STAMAC[5]);
      Page_SendInfoElement(sm, "Station MAC", buf);
    }

  snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u",
	   sEsp.APIP[0], sEsp.APIP[1],
	   sEsp.APIP[2], sEsp.APIP[3]);
  Page_SendInfoElement(sm, "AP IP", buf);
  snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u",
	   sEsp.APGateway[0], sEsp.APGateway[1],
	   sEsp.APGateway[2], sEsp.APGateway[3]);
  Page_SendInfoElement(sm, "AP Gateway", buf);
  snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u",
	   sEsp.APNetmask[0], sEsp.APNetmask[1],
	   sEsp.APNetmask[2], sEsp.APNetmask[3]);
  Page_SendInfoElement(sm, "AP Netmask", buf);
  snprintf(buf, LOCAL_BUFFER_SIZE, "%02X:%02X:%02X:%02X:%02X:%02X",
	   sEsp.APMAC[0], sEsp.APMAC[1],
	   sEsp.APMAC[2], sEsp.APMAC[3],
	   sEsp.APMAC[4], sEsp.APMAC[5]);
  Page_SendInfoElement(sm, "AP MAC", buf);

  Http_HelperSendMessageBody(sm, "</div>");

  Page_SendButton(conn, "Disconnect", "loadDisconnect()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
