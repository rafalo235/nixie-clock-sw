/*
 * connection-task.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Rafal Olejniczak
 */

#include "connection-task/connection-task.h"
#include "esp/esp.h"
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
#include "esp/esp.h"
#include "system/esp_ll.h"
#include "esp/esp_netconn.h"


esp_ll_t gEsp;
char gConnectApn[32];
char gConnectPassword[32];
int gConnectFlag = 0;
int gDisconnectFlag = 0;

static espr_t
Connection_Callback(esp_evt_t* evt);

void Connection_Task(void *parameters)
{
  espr_t res;
  esp_netconn_p server, client;

  if ((res = esp_init(&Connection_Callback, 1)) == espOK) {
    asm volatile ("nop");
  }

#if 1
  if ((res = esp_sta_join(WIFI_NAME, WIFI_PASS, NULL, 0, 1)) == espOK) {
    esp_ip_t ip;
    esp_sta_copy_ip(&ip, NULL, NULL);
    Connection_SetConnected(1);
  }
  else
  {
    Connection_SetConnected(0);
  }
#endif

#if 0
  SNTP_Initialize();

#endif

  server = esp_netconn_new(ESP_NETCONN_TYPE_TCP);
  if (NULL == server)
  {
    configASSERT(0); /* fixme remove asserts */
  }
  res = esp_netconn_bind(server, 80);
  if (res != espOK)
  {
    configASSERT(0);
  }
  res = esp_netconn_listen(server);
  if (res != espOK)
  {
    configASSERT(0);
  }

  while (1)
  {
    res = esp_netconn_accept(server, &client);
    if (res == espOK)
    {
      static tuCHttpServerState sServerState;
      esp_pbuf_p pbuf, prev;

      Http_InitializeConnection(
          &sServerState, &Http_SendPort, &OnError,
          &resources, 10, client);

      esp_netconn_set_receive_timeout(client, 500);

      do
      {
        res = esp_netconn_receive(client, &pbuf);

        if (espOK == res)
        {
          while (NULL != pbuf)
          {
            Http_Input(&sServerState, esp_pbuf_data(pbuf), esp_pbuf_length(pbuf, 0u));
            prev = pbuf;
            pbuf = esp_pbuf_unchain(prev);
            esp_pbuf_free(prev);
          }
          esp_netconn_flush(client);
        }

      } while (espOK == res);
      /* After this loop connection is closed */

      /* Make sure connection is closed ?todo is necessary */
      esp_netconn_close(client);
      esp_netconn_delete(client);
    }

  }
}

static espr_t Connection_Callback(esp_evt_t* evt)
{
  espr_t res;
  switch (esp_evt_get_type(evt))
  {
  case ESP_EVT_INIT_FINISH:
  {
    break;
  }
  case ESP_EVT_RESET_FINISH:
  {
    break;
  }
  case ESP_EVT_RESET:
  {
    break;
  }
  case ESP_EVT_WIFI_CONNECTED:
  {
    break;
  }
  case ESP_EVT_WIFI_DISCONNECTED:
  {
    break;
  }
  default:
    break;
  }
  ESP_UNUSED(res);
  return espOK;
}
