/*
 access-points.c

  Created on: Apr 5, 2018
      Author: Rafał Olejniczak
 */

#include "resources/pages.h"
#include "resources/common.h"

tHttpStatusCode AccessPointsCallback(void * const conn)
{
  tuCHttpServerState * const sm = conn;
  ESP_Result_t espResult;
  uint16_t res;
  ESP_AP_t aps[16];
  ESP_AP_t * ptr = aps;

  /* Fetch Access points */
  if (espOK != (espResult = ESP_STA_ListAccessPoints(&sEsp, aps, 16, &res, 1)))
    {
      asm volatile ("nop");
    }

  /* Send header */
  Http_HelperSetResponseStatus(sm, HTTP_STATUS_OK);
  Http_HelperSetResponseHeader(sm, "Content-Type", "application/json");
  Http_HelperSendHeader(sm);

  /* Create and send content */
  Json_OpenObject(conn);

  Json_Attribute(conn, "acess_points");
  Json_OpenArray(conn);
  while (res--)
    {
      Json_OpenObject(conn);

      Json_Attribute(conn, "value");
      Json_Value(conn, ptr->SSID);

      Json_Separator(conn);

      Json_Attribute(conn, "display");
      Json_Value(conn, ptr->SSID);

      Json_CloseObject(conn);
      if (0 != res)
	{
	  Json_Separator(conn);
	}
      ++ptr;
    }
  Json_CloseArray(conn);

  Json_CloseObject(conn);

  Http_HelperFlush(sm);

  Disconnect(&sEsp, Http_HelperGetContext(conn));

  return HTTP_STATUS_OK;
}
