/*
 * connection-task.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Rafal Olejniczak
 */

#include "connection-task/connection-task.h"
#include "esp8266.h"
#include "drivers/usart/usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f103xb.h"
#include "uchttpserver.h"
#include "port/uchttpserver/port.h"
#include "port/uchttpserver/resources.h"
#include "resources/common.h"
#include "resources/pages.h"
#include "resources/routine.h"
#include "resources/connection-routines.h"

int EspCallback(ESP_Event_t, ESP_EventParams_t *);

volatile ESP_t sEsp;
char gConnectApn[32];
char gConnectPassword[32];
int gConnectFlag = 0;
int gDisconnectFlag = 0;

#define NUM_CONNECTIONS 5

tuCHttpServerState connection[NUM_CONNECTIONS];
ESP_CONN_t * connectionPcb[NUM_CONNECTIONS] = {
    NULL, NULL, NULL, NULL, NULL
};

tuCHttpServerState * GetServer(ESP_CONN_t * ctx)
{
  tuCHttpServerState * result = NULL;
  int i = 0;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (connectionPcb[i] == ctx)
    {
      result = &(connection[i]);
      break;
    }
  }
  if (NULL == result)
  {
    for (i = 0; i < NUM_CONNECTIONS; ++i)
    {
      if (connectionPcb[i] == NULL)
      {
        result = &(connection[i]);
        connectionPcb[i] = ctx;

        Http_InitializeConnection(
            result, &Http_SendPort, &OnError,
            &resources, 8, ctx);
        break;
      }
    }
  }

  return result;
}

void ReleaseServer(ESP_CONN_t * ctx)
{
  int i = 0;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (connectionPcb[i] == ctx)
    {
      connectionPcb[i] = NULL;
      break;
    }
  }
}

void ReleaseAllAndDisconnect(void)
{
  int i;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (NULL != connectionPcb[i])
    {
      ESP_CONN_Close(&sEsp, connectionPcb[i], 1);
      connectionPcb[i] = NULL;
    }
  }

}

void Connection_Task(void *parameters)
{

  ESP_Result_t espResult;

  if (espOK != (espResult = ESP_Init(&sEsp, 115200, EspCallback)))
  {
    asm volatile ("nop");
  }
#if 1
  if (espOK
      == (espResult = ESP_STA_Connect(&sEsp, WIFI_NAME, WIFI_PASS, NULL, 0, 1)))
  {
    Connection_SetConnected(1);
  } else
  {
    Connection_SetConnected(0);
  }
#endif

#if 0
  /* Set access point */
  sEsp.APConf.Hidden = 0; /* Allow AP to be seen in network */
  sEsp.APConf.MaxConnections = 3; /* Allow up to 3 devices connected to AP at a time */
  strcpy((char *)sEsp.APConf.SSID, "Nixie-Clock"); /* Set AP name */
  strcpy((char *)sEsp.APConf.Pass, "nixie"); /* Set AP password */
  sEsp.APConf.Ecn = ESP_Ecn_WPA2_PSK; /* Set security level */

  /* Set config */
  if (espOK != (espResult = ESP_AP_SetConfig(&sEsp, (ESP_APConfig_t *)&sEsp.APConf, 0, 1)))
  {
    asm volatile ("nop");
  }

#endif

  if (espOK != (espResult = ESP_SERVER_Enable(&sEsp, 80, 0)))
  {
    asm volatile ("nop");
  }

  while (1)
  {
    uint32_t tim;
    ESP_Update(&sEsp);
    ESP_ProcessCallbacks(&sEsp);

    Routine_ExecuteRoutine(&gConnectionRoutine);

    // todo remove
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

unsigned int Http_SendPort(void * const conn, const char * data,
    unsigned int length);

int EspCallback(ESP_Event_t evt, ESP_EventParams_t* params)
{
  ESP_CONN_t* conn;
  uint8_t* data;
  tuCHttpServerState * server;

  switch (evt)
  { /* Check events */
  case espEventIdle:
    break;
  case espEventConnActive:
  {
    conn = (ESP_CONN_t *) params->CP1; /* Get connection for event */

    server = GetServer(conn);
    if (NULL == server)
    {
      ESP_CONN_Close(&sEsp, conn, 1);
    }
    break;
  }
  case espEventConnClosed:
  {
    conn = (ESP_CONN_t *) params->CP1; /* Get connection for event */
    ReleaseServer(conn);
    break;
  }
  case espEventDataReceived:
  {
    conn = (ESP_CONN_t *) params->CP1; /* Get connection for event */
    data = (uint8_t *) params->CP2; /* Get data */

    server = GetServer(conn);
    if (NULL != server)
    {
      Http_Input(server, data, strlen(data));
    } else
    {
      ESP_CONN_Close(&sEsp, conn, 1);
    }
    break;
  }
  case espEventDataSent:
    conn = (ESP_CONN_t *) params->CP1; /* Get connection for event */
    break;
  case espEventDataSentError:
    conn = (ESP_CONN_t *) params->CP1; /* Get connection for event */
    ESP_CONN_Close(&sEsp, conn, 1); /* fixme ? set connectionNumber */
    break;
  default:
    break;
  }

  return 0;
}

void vApplicationTickHook(void)
{
  static uint16_t sCounter = pdMS_TO_TICKS(1);

  if (sReceiverEnabled)
  {
    if (--sCounter == 0)
    {
      ESP_UpdateTime(&sEsp, 1);
      sCounter = pdMS_TO_TICKS(1);
    }
  }
}
