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

int EspCallback(ESP_Event_t, ESP_EventParams_t *);

volatile ESP_t sEsp;

void Connection_Task(void *parameters)
{

#if 0
  const char *p;
#define	AT	"AT\r\n"
#define AT_CWMODE	"AT+CWMODE=1\r\n"
#define AT_CWJAP	"AT+CWJAP="## WIFI_NAME ##","## WIFI_PASS ##"\r\n"
#define	AT_RST	"AT+RST\r\n"
#define	AT_CWLAP	"AT+CWLAP\r\n"
#define	AT_CWDHCP	"AT+CWDHCP=1,1\r\n"

  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRH = (GPIOA->CRH & ~(GPIO_CRH_CNF11 | GPIO_CRH_MODE11)) |
      GPIO_CRH_MODE11_1;
  /* Set standby high */
  GPIOA->BSRR = GPIO_BSRR_BS11;

  p = SendAndReceive(AT_RST, sizeof(AT_RST));
  (void)p;

  p = SendAndReceive(AT, sizeof(AT));
  (void)p;

  p = SendAndReceive(AT_CWMODE, sizeof(AT_CWMODE));
  (void)p;

  p = SendAndReceive(AT_CWLAP, sizeof(AT_CWLAP));
  (void)p;

  SendAndReceive(AT_CWDHCP, sizeof(AT_CWDHCP));
  (void)p;

  SendAndReceive(AT_CWJAP, sizeof(AT_CWJAP));
  (void)p;

#else

  ESP_Result_t espResult;

  if (espOK != (espResult = ESP_Init(&sEsp, 115200, EspCallback)))
    {
      asm volatile ("nop");
    }
#if 0
  if (espOK == (espResult = ESP_STA_Connect(
	&sEsp, WIFI_NAME, WIFI_PASS, NULL, 0, 1)))
    {
      Connection_SetConnected(1);
    }
  else
    {
      Connection_SetConnected(0);
    }
#endif

#if 0
  /* Set access point */
  sEsp.APConf.Hidden = 0;                  /* Allow AP to be seen in network */
  sEsp.APConf.MaxConnections = 3;   /* Allow up to 3 devices connected to AP at a time */
  strcpy((char *)sEsp.APConf.SSID, "Nixie-Clock"); /* Set AP name */
  strcpy((char *)sEsp.APConf.Pass, "nixie");  /* Set AP password */
  sEsp.APConf.Ecn = ESP_Ecn_WPA2_PSK;      /* Set security level */

  /* Set config */
  if (espOK != (espResult = ESP_AP_SetConfig(&sEsp, (ESP_APConfig_t *)&sEsp.APConf, 0, 1)))
    {
      asm volatile ("nop");
    }

  if (espOK != (espResult = ESP_SERVER_Enable(&sEsp, 80, 1)))
    {
      asm volatile ("nop");
    }

  while (1)
    {
      ESP_ProcessCallbacks(&sEsp);

    }
#endif

  if (espOK != (espResult = ESP_SetHostName(&sEsp, "nixie", 1)))
    {
      asm volatile ("nop");
    }

  if (espOK != (espResult = ESP_SERVER_Enable(&sEsp, 80, 0)))
    {
      asm volatile ("nop");
    }

  while (1)
    {
      uint32_t tim;
      ESP_ProcessCallbacks(&sEsp);

#if 0
      if (espOK != (espResult = ESP_Ping(&sEsp, "192.168.128.66", &tim, 1)))
        {
          asm volatile ("nop");
        }
#endif
      vTaskDelay (pdMS_TO_TICKS (10));
    }
#endif

}

unsigned int Http_SendPort(
    void * const  conn, const char * data, unsigned int length);

#define NUM_CONNECTIONS 5

tuCHttpServerState connection[NUM_CONNECTIONS];
int connectionNumber[NUM_CONNECTIONS] = { -1, -1, -1, -1, -1 };

static tuCHttpServerState * GetServer(ESP_CONN_t * ctx, int conn)
{
  tuCHttpServerState * result = NULL;
  int i = 0;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (connectionNumber[i] == conn)
    {
      result = &(connection[i]);
    }
  }
  if (NULL == result)
  {
    for (i = 0; i < NUM_CONNECTIONS; ++i)
    {
      if (connectionNumber[i] == (-1))
      {
        result = &(connection[i]);
        connectionNumber[i] = conn;

        Http_InitializeConnection(
            result, &Http_SendPort, &OnError,
            &resources, 8, ctx);
      }
    }
  }

  return result;
}

static void ReleaseServer(int conn)
{
  int i = 0;

  for (i = 0; i < NUM_CONNECTIONS; ++i)
  {
    if (connectionNumber[i] == conn)
    {
      connectionNumber[i] = -1;
    }
  }
}

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

    server = GetServer(conn, conn->Number);
    if (NULL == server)
    {
      ESP_CONN_Close(&sEsp, conn, 1);
    }
    break;
  }
  case espEventConnClosed:
  {
    conn = (ESP_CONN_t *) params->CP1; /* Get connection for event */
    ReleaseServer(conn->Number);
    break;
  }
  case espEventDataReceived:
  {
    conn = (ESP_CONN_t *) params->CP1; /* Get connection for event */
    data = (uint8_t *) params->CP2; /* Get data */

    server = GetServer(conn, conn->Number);
    if (NULL != server)
    {
      Http_Input(server, data, strlen(data));
    }
    else
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

void Update_Task(void *param)
{
  while (1)
    {
      ESP_Update(&sEsp);
    }
}

void vApplicationTickHook( void )
{
  static uint16_t sCounter = pdMS_TO_TICKS (1);

  if (sReceiverEnabled)
    {
      if (--sCounter == 0)
	{
	  ESP_UpdateTime (&sEsp, 1);
	  sCounter = pdMS_TO_TICKS (1);
	}
    }
}
