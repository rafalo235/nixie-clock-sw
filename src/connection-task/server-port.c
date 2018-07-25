/*
 server-port.c

  Created on: Jul 16, 2018
      Author: Rafał Olejniczak
 */

#include "connection-task/server-port.h"
#include "esp/esp.h"
#include "uchttpserver.h"
#include "port/uchttpserver/port.h"
#include "resources/pages.h"
#include "port/uchttpserver/resources.h"
#include "connection-task/command-dispatcher.h"

#define NUM_CONNECTIONS 5

tuCHttpServerState connection[NUM_CONNECTIONS];
esp_conn_p connectionPcb[NUM_CONNECTIONS] = {
    NULL, NULL, NULL, NULL, NULL
};

static tuCHttpServerState * AllocateServer(esp_conn_p conn);

static tuCHttpServerState * GetServer(esp_conn_p conn);

static void ReleaseServer(esp_conn_p conn);

espr_t Server_Callback(esp_evt_t* evt)
{
  esp_conn_p conn;
  tuCHttpServerState * server;

  conn = esp_conn_get_from_evt(evt); /* Get connection handle from event */
  switch (esp_evt_get_type(evt))
  {
  case ESP_EVT_CONN_ACTIVE:
  { /* Connection just active */
    int num = (int)esp_conn_getnum(conn);
    server = AllocateServer(conn);

    if (NULL != server)
    {
      Http_InitializeConnection(
          server, &Http_SendPort, &OnError,
          &resources, 10, conn);
    }
    else
    {
      Dispatcher_Send(COMMAND_CLOSE_SERVER_CONNECTION, conn);
    }

    break;
  }
  case ESP_EVT_CONN_DATA_RECV:
  { /* Connection data received */
    esp_pbuf_p p;
    p = esp_evt_conn_data_recv_get_buff(evt); /* Get received buffer */
    if (p != NULL)
    {
      const void * data = esp_pbuf_data(p);
      int num = (int)esp_conn_getnum(conn);
      int len = (int) esp_pbuf_length(p, 1);
      server = GetServer(conn);

      if (NULL != server)
      {
        Dispatcher_SetDataBuffer(data, len);
        Dispatcher_Send(COMMAND_HANDLE_REQUEST, server);
      }
      else
      {
        Dispatcher_Send(COMMAND_CLOSE_SERVER_CONNECTION, conn);
      }
    }
    break;
  }
  case ESP_EVT_CONN_CLOSED:
  { /* Connection closed */
    ReleaseServer(conn);
    break;
  }
  }
  return espOK;
}

tuCHttpServerState * AllocateServer(esp_conn_p conn)
{
  tuCHttpServerState * result = NULL;
  int i = 0;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (NULL == connectionPcb[i])
    {
      connectionPcb[i] = conn;
      result = &connection[i];
      break;
    }
  }

  return result;
}

tuCHttpServerState * GetServer(esp_conn_p conn)
{
  tuCHttpServerState * result = NULL;
  int i = 0;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (connectionPcb[i] == conn)
    {
      result = &(connection[i]);
      break;
    }
  }

  return result;
}

void ReleaseServer(esp_conn_p conn)
{
  int i = 0;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (connectionPcb[i] == conn)
    {
      connectionPcb[i] = NULL;
      break;
    }
  }
}
