/*
 access-points.c

  Created on: Apr 5, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"
#include "esp/esp.h"
#include "esp/esp_sta.h"

#define MAX_ACCESS_POINTS   16

esp_ap_t sAps[MAX_ACCESS_POINTS];

tHttpStatusCode AccessPointsCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  espr_t result;
  size_t accessPointsNumber;
  esp_ap_t * ptr = sAps;

  /* Fetch Access points */
  result = esp_sta_list_ap(NULL, &sAps[0],
  MAX_ACCESS_POINTS, &accessPointsNumber, 1u);

  /* Send header */
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "application/json");
  Http_HelperSendHeader(sm);

  /* Create and send content */
  Json_OpenObject(conn);

  Json_Attribute(conn, "acess_points");
  Json_OpenArray(conn);
  while (accessPointsNumber--)
  {
    Json_OpenObject(conn);

    Json_Attribute(conn, "value");
    Json_Value(conn, ptr->ssid);

    Json_Separator(conn);

    Json_Attribute(conn, "display");
    Json_Value(conn, ptr->ssid);

    Json_CloseObject(conn);
    if (0 != accessPointsNumber)
    {
      Json_Separator(conn);
    }
    ++ptr;
  }
  Json_CloseArray(conn);

  Json_CloseObject(conn);

  Http_HelperFlush(sm);

  return HTTP_STATUS_OK;
}
