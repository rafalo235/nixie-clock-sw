/*
  connection-routines.c

  Created on: May 31, 2018
      Author: Rafał Olejniczak
 */

#include "resources/connection-routines.h"
#include "resources/common.h"
#include "esp/esp.h"

tRoutine gConnectionRoutine;

int ConnectToAccessPoint(void * param)
{
#if 0
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

#endif
  return (int)0;
}

int DisconnectFromAccessPoint(void * param)
{
  tConnectionRoutinesResults result;

  if (1 == Connection_IsConnected())
  {
    espr_t result;

    if (espOK == (result = esp_sta_quit(1u)))
    {
      result = CONN_ROUTINE_WIFI_DISCONNECTED;
    }
    else
    {
      result = CONN_ROUTINE_WIFI_DISCONNECTION_ERROR;
    }
    Connection_SetConnected(0);
  }
  else
  {
    result = CONN_ROUTINE_WIFI_NOT_CONNECTED;
  }

  return (int)result;
}

int SetSNTPConfig(void * param)
{
  tConnectionRoutinesResults result;
#if 0
  ESP_Result_t espResult;

  /* Address and timezone is already there */
  SNTP_GetConfig()->Enable = 1;

  espResult = ESP_SNTP_SetConfig(&sEsp, SNTP_GetConfig(), 1);
  if (espOK == espResult)
  {
    result = CONN_ROUTINE_SNTP_CONFIGURED;
  }
  else
  {
    result = CONN_ROUTINE_SNTP_CONFIG_ERROR;
  }
#endif

  return (int)result;
}

int SynchronizeRoutine(void * param)
{
  tConnectionRoutinesResults result;
#if 0
  ESP_Result_t espResult;
  ESP_DateTime_t dt;

  espResult = ESP_SNTP_GetDateTime(&sEsp, &dt, 1);
  if (espOK == espResult)
  {
    result = CONN_ROUTINE_SNTP_SYNCHRONIZATION_SUCCESSFUL;
  }
  else
  {
    result = CONN_ROUTINE_SNTP_SYNCHRONIZATION_ERROR;
  }
#endif

  return (int)result;
}

const char * ResolveResultMessage(
    tConnectionRoutinesResults result)
{
  const char * info;

  switch (result)
  {
  case CONN_ROUTINE_WIFI_CONNECTED :
    info = "Connection successful";
    break;
  case CONN_ROUTINE_WIFI_WRONG_CREDENTIALS :
    info = "Wrong password, try again";
    break;
  case CONN_ROUTINE_WIFI_CONNECTION_ERROR :
    info = "Error occurred during connection setup";
    break;
  case CONN_ROUTINE_WIFI_ALREADY_CONNECTED :
    info = "You are already connected to Access Point";
    break;
  case CONN_ROUTINE_WIFI_DISCONNECTED :
    info = "Disconnected from Access Point successfully";
    break;
  case CONN_ROUTINE_WIFI_NOT_CONNECTED :
    info = "You are not connected to Access Point";
    break;
  case CONN_ROUTINE_WIFI_DISCONNECTION_ERROR :
    info = "Disconnection successful";
    break;
  case CONN_ROUTINE_SNTP_CONFIGURED :
    info = "SNTP configuration successful";
    break;
  case CONN_ROUTINE_SNTP_CONFIG_ERROR :
    info = "Error occurred during SNTP configuration";
    break;
  case CONN_ROUTINE_SNTP_SYNCHRONIZATION_SUCCESSFUL :
    info = "Synchronization successful";
    break;
  case CONN_ROUTINE_SNTP_SYNCHRONIZATION_ERROR :
    info = "Error occurred during SNTP synchronization";
    break;
  default:
    info = "Unknown event occured";
    break;
  }

  return info;
}
