/*
 server-port.c

  Created on: Jul 16, 2018
      Author: Rafał Olejniczak
 */

#include "connection-task/server-port.h"
#include "esp/esp.h"
#include "uchttpserver.h"

#define NUM_CONNECTIONS 5

tuCHttpServerState connection[NUM_CONNECTIONS];
int * connectionPcb[NUM_CONNECTIONS] = {
    NULL, NULL, NULL, NULL, NULL
};


espr_t Server_Callback(esp_evt_t* evt)
{
  return espOK;
}

#if 0
tuCHttpServerState * GetServer(int * ctx)
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
            &resources, 10, ctx);
        break;
      }
    }
  }

  return result;
}

void ReleaseServer(int * ctx)
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
#if 0
      ESP_CONN_Close(&sEsp, connectionPcb[i], 1);
#endif
      connectionPcb[i] = NULL;
    }
  }

}
#endif
