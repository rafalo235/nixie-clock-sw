/*
  connection-routines.c

  Created on: May 31, 2018
      Author: Rafał Olejniczak
 */

#include "resources/connection-routines.h"
#include "resources/common.h"
#include "esp8266.h"

tRoutine gConnectionRoutine;

int ConnectToAccessPoint(void * param)
{
  tConnectionCredentials * credentials = param;
  tConnectionRoutinesResults result;

  if (0 == Connection_IsConnected())
  {
    ESP_Result_t espResult;
    ReleaseAllAndDisconnect();
    if (espOK
        == (espResult = ESP_STA_Connect(&sEsp,
            credentials->apn, credentials->password,
            NULL, 0, 1)))
    {
      Connection_SetConnected(1);
      result = CONN_ROUTINE_WIFI_CONNECTED;
    }
    else
    {
      if (espWRONGPASSWORD == espResult)
      {
        result = CONN_ROUTINE_WIFI_WRONG_CREDENTIALS;
      }
      else
      {
        result = CONN_ROUTINE_WIFI_CONNECTION_ERROR;
      }
      Connection_SetConnected(0);
    }
  }
  else
  {
    result = CONN_ROUTINE_WIFI_ALREADY_CONNECTED;
  }

  return (int)result;
}

int DisconnectFromAccessPoint(void * param)
{
  tConnectionRoutinesResults result;

  if (1 == Connection_IsConnected())
  {
    ESP_Result_t espResult;

    ReleaseAllAndDisconnect();
    if (espOK == (espResult = ESP_STA_Disconnect(&sEsp, 1)))
    {
      result = CONN_ROUTINE_WIFI_DISCONNECTED;
      Connection_SetConnected(0);
    }
    else
    {
      result = CONN_ROUTINE_WIFI_DISCONNECTION_ERROR;
    }
  }
  else
  {
    result = CONN_ROUTINE_WIFI_NOT_CONNECTED;
  }

  return (int)result;
}
