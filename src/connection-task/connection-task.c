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
#endif

  Routine_Init(&gConnectionRoutine);

  while (1)
  {
    uint32_t tim;
#if 0
    ESP_Update(&sEsp);
    ESP_ProcessCallbacks(&sEsp);

    Routine_ExecuteRoutine(&gConnectionRoutine);
#endif

    // todo remove
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

static espr_t
Connection_Callback(esp_evt_t* evt)
{
  espr_t res;
  switch (esp_evt_get_type(evt)) {
      case ESP_EVT_INIT_FINISH: {
          break;
      }
      case ESP_EVT_RESET_FINISH: {
          break;
      }
      case ESP_EVT_RESET: {
          break;
      }
      case ESP_EVT_WIFI_CONNECTED: {
          /* Start server on port 80 and set callback for new connections */
          if ((res = esp_set_server(1, 80, ESP_CFG_MAX_CONNS, 100, Server_Callback, 0)) == espOK) {
              asm volatile ("nop");
          }
          break;
      }
      case ESP_EVT_WIFI_DISCONNECTED: {
          /* Stop server on port 80, others parameters are don't care */
          printf("Wifi disconnected\r\n");
          if ((res = esp_set_server(0, 80, ESP_CFG_MAX_CONNS, 100, Server_Callback, 0)) == espOK) {
            asm volatile ("nop");
          }
          break;
      }
      default: break;
  }
  ESP_UNUSED(res);
  return espOK;
}
