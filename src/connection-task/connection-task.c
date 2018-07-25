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
#include "connection-task/server-port.h"
#include "connection-task/command-dispatcher.h"


esp_ll_t gEsp;
char gConnectApn[32];
char gConnectPassword[32];
int gConnectFlag = 0;
int gDisconnectFlag = 0;

static espr_t
Connection_Callback(esp_evt_t* evt);

void Connection_Task(void *parameters)
{
  espr_t espResult;

  Dispatcher_Init();

  if ((espResult = esp_init(&Connection_Callback, 1)) == espOK) {
    asm volatile ("nop");
  }

#if 1
  if ((espResult = esp_sta_join(WIFI_NAME, WIFI_PASS, NULL, 0, 1)) == espOK) {
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

  Routine_Init(&gConnectionRoutine);

  while (1)
  {
    espr_t res;
    tCommand command = Dispatcher_Wait();

    if (COMMAND_START_SERVER == command.type)
    {
      if ((res = esp_set_server(1, 80, ESP_CFG_MAX_CONNS, 100, &Server_Callback, 1)) == espOK) {
          asm volatile ("nop");
      }
    }
    else if (COMMAND_HANDLE_REQUEST == command.type)
    {
      if (0u != Dispatcher_GetDataBufferSize())
      {
        Http_Input((tuCHttpServerState*)command.parameter,
            Dispatcher_GetDataBuffer(), Dispatcher_GetDataBufferSize());
      }
    }
    else if (COMMAND_CLOSE_SERVER_CONNECTION == command.type)
    {
      esp_conn_close((esp_conn_p)command.parameter, 1u);
    }
    else if (COMMAND_STOP_SERVER == command.type)
    {
      if ((res = esp_set_server(0, 80, ESP_CFG_MAX_CONNS, 100, &Server_Callback, 1)) == espOK) {
        asm volatile ("nop");
      }
    }

#if 0
    Routine_ExecuteRoutine(&gConnectionRoutine);
#endif
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
    /* Start server on port 80 and set callback for new connections */
    Dispatcher_Send(COMMAND_START_SERVER, NULL);
    break;
  }
  case ESP_EVT_WIFI_DISCONNECTED:
  {
    Dispatcher_Send(COMMAND_STOP_SERVER, NULL);
    break;
  }
  default:
    break;
  }
  ESP_UNUSED(res);
  return espOK;
}
