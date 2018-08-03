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
#include "control-task/control-task.h"
#include "connection-task/configuration.h"

typedef enum ConnectionResult
{
  CONNECTION_RESULT_OK,
  CONNECTION_SERVER_BROKEN,
  CONNECTION_SERVER_OUT_OF_RESOURCES,
  CONNECTION_SERVER_BIND_ERROR,
  CONNECTION_SERVER_LISTEN_ERROR
} tConnectionResult;

static espr_t Connection_Callback(esp_evt_t* evt);

static tConnectionResult Connection_RunServer(void);

static void Connection_HandleClient(esp_netconn_p client);

void Connection_Task(void *parameters)
{
  espr_t res;
  tConnectionResult connResult;

  if ((res = esp_init(&Connection_Callback, 1)) == espOK) {
    asm volatile ("nop");
  }

  Configuration_Init();

  if (Configuration_Get(&gConfigLocal))
  {
    if ((res = esp_sta_join(gConfigLocal.apn,
        gConfigLocal.password, NULL, 0, 1)) == espOK)
    {
      const tControlAction action = CONTROL_ACTION_SHOW_IP;

      Connection_SetConnected(1);

      xQueueSendToBack(gControlQueue, &action, portMAX_DELAY);
    }
    else
    {
      Connection_SetConnected(0);
    }

    /* Configure SNTP settings from flash */
    esp_sntp_configure(1, gConfigLocal.timezone, gConfigLocal.sntp[0],
        gConfigLocal.sntp[1], gConfigLocal.sntp[2], 1u);
  }
  else
  {
    Connection_SetConnected(0);
  }

  Routine_Init(&gConnectionRoutine);

  /* ---- Start connection task loop ---- */
  while (1)
  {
    connResult = Connection_RunServer();
    if (CONNECTION_SERVER_BROKEN != connResult)
    {
      /* Wait for resources if trivial functions fails */
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
  /* ---- Start connection task loop ---- */
}

static tConnectionResult Connection_RunServer(void)
{
  espr_t res;
  tConnectionResult result;
  esp_netconn_p server, client;

  server = esp_netconn_new(ESP_NETCONN_TYPE_TCP);
  if (NULL == server)
  {
    result = CONNECTION_SERVER_OUT_OF_RESOURCES;
  }
  else
  {
    res = esp_netconn_bind(server, 80);
    if (res != espOK)
    {
      result = CONNECTION_SERVER_BIND_ERROR;
    }
    else
    {
      res = esp_netconn_listen(server);
      if (res != espOK)
      {
        result = CONNECTION_SERVER_LISTEN_ERROR;
      }
      else
      {

        /* ---- Start main server loop ---- */
        while (1)
        {
          res = esp_netconn_accept(server, &client);
          if (res == espOK)
          {
            Connection_HandleClient(client);
          }
          else
          {
            /* Server netconn is gone */
            esp_netconn_delete(server);
            break;
          }
        }
        /* ---- End main server loop ---- */
        result = CONNECTION_SERVER_BROKEN;
      }
    }
  }

  return result;
}

static void Connection_HandleClient(esp_netconn_p client)
{
  espr_t res;
  static tuCHttpServerState sServerState;
  esp_pbuf_p pbuf, prev;

  Http_InitializeConnection(&sServerState, &Http_SendPort,
      &OnError, &resources, 11, client);

  esp_netconn_set_receive_timeout(client, 500);

  do
  {
    res = esp_netconn_receive(client, &pbuf);

    if (espOK == res)
    {
      while (NULL != pbuf)
      {
        Http_Input(&sServerState, esp_pbuf_data(pbuf),
            esp_pbuf_length(pbuf, 0u));
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

  Routine_ExecuteRoutine(&gConnectionRoutine);

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
  case ESP_EVT_WIFI_GOT_IP:
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
