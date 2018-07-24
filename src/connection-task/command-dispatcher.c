/*
 command-dispatcher.c

  Created on: Jul 24, 2018
      Author: Rafał Olejniczak
 */

#include "connection-task/command-dispatcher.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof(tCommand)

static StaticQueue_t xStaticQueue;
static uint8_t ucQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE ];
static QueueHandle_t sQueueHandle;

void Dispatcher_Init(void)
{
  sQueueHandle = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE,
      ucQueueStorageArea, &xStaticQueue );
}

void Dispatcher_Send(tCommand command)
{
  xQueueSend(sQueueHandle, &command, 0);
}

tCommand Dispatcher_Wait(void)
{
  tCommand command;

  xQueueReceive(sQueueHandle, &command, portMAX_DELAY);

  return command;
}
