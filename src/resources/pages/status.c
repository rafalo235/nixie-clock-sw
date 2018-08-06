/*
 status.c

 MIT License

  Created on: Apr 18, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "esp/esp.h"
#include "esp/esp_sta.h"
#include "esp/esp_ap.h"
#include "connection-task/configuration.h"

#define LOCAL_BUFFER_SIZE	20

tHttpStatusCode StatusCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  char buf[LOCAL_BUFFER_SIZE];
  uint8_t isConnected = Connection_IsConnected();
  esp_mac_t smac;
  esp_ip_t sip, sgw, snm;
  esp_mac_t amac;
  esp_ip_t aip, agw, anm;

  if (isConnected)
  {
    esp_sta_copy_ip(&sip, &sgw, &snm);
    esp_sta_getmac(&smac, 0u, 1u);
  }

  esp_ap_getip(&aip, &agw, &anm, 0u, 1u);
  esp_ap_getmac(&amac, 0u, 1u);

  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "text/html");
  Http_HelperSendHeader(sm);

  Http_HelperSendMessageBody(sm, "<html>");
  Page_SendHtmlHeader(conn, "Connection Status");
  Http_HelperSendMessageBody(sm, "<body>");

  Http_HelperSendMessageBody(sm, "<div class=\"info\">");

  /* Info elements */
  if (isConnected)
  {
    Page_SendInfoElement(sm, "Access point", gConfigLocal.apn);

    snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u", sip.ip[0], sip.ip[1],
        sip.ip[2], sip.ip[3]);
    Page_SendInfoElement(sm, "Station IP", buf);

    snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u", sgw.ip[0], sgw.ip[1],
        sgw.ip[2], sgw.ip[3]);
    Page_SendInfoElement(sm, "Station Gateway", buf);

    snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u", snm.ip[0], snm.ip[1],
        snm.ip[2], snm.ip[3]);
    Page_SendInfoElement(sm, "Station Netmask", buf);

    snprintf(buf, LOCAL_BUFFER_SIZE, "%02X:%02X:%02X:%02X:%02X:%02X",
        smac.mac[0], smac.mac[1], smac.mac[2], smac.mac[3], smac.mac[4],
        smac.mac[5]);
    Page_SendInfoElement(sm, "Station MAC", buf);
  }

  snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u", aip.ip[0], aip.ip[1],
      aip.ip[2], aip.ip[3]);
  Page_SendInfoElement(sm, "AP IP", buf);

  snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u", agw.ip[0], agw.ip[1],
      agw.ip[2], agw.ip[3]);
  Page_SendInfoElement(sm, "AP Gateway", buf);

  snprintf(buf, LOCAL_BUFFER_SIZE, "%u.%u.%u.%u", anm.ip[0], anm.ip[1],
      anm.ip[2], anm.ip[3]);
  Page_SendInfoElement(sm, "AP Netmask", buf);

  snprintf(buf, LOCAL_BUFFER_SIZE, "%02X:%02X:%02X:%02X:%02X:%02X", amac.mac[0],
      amac.mac[1], amac.mac[2], amac.mac[3], amac.mac[4], amac.mac[5]);
  Page_SendInfoElement(sm, "AP MAC", buf);

  Http_HelperSendMessageBody(sm, "</div>");

  if (isConnected)
  {
    Page_SendButton(conn, "Disconnect", "loadDisconnect()");
  }
  Page_SendButton(conn, "Back", "loadIndex()");

  Http_HelperSendMessageBody(sm, "</body>");
  Http_HelperSendMessageBody(sm, "</html>");
  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
