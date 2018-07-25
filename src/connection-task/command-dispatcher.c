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

#define DISPATCHER_DATA_BUFFER_LENGTH 1024

static StaticQueue_t xStaticQueue;
static uint8_t ucQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE ];
static QueueHandle_t sQueueHandle;

static char sDataBuffer[DISPATCHER_DATA_BUFFER_LENGTH];
static unsigned int sDataBufferSize = 0u;

void Dispatcher_Init(void)
{
  sQueueHandle = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE,
      ucQueueStorageArea, &xStaticQueue );
}

void Dispatcher_Send(tCommandType type, void * parameter)
{
  tCommand command;
  command.type = type;
  command.parameter = parameter;

  xQueueSend(sQueueHandle, &command, 0);
}

int Dispatcher_SetDataBuffer(const void * buffer, unsigned int length)
{
  int result = 0;
  if (length <= DISPATCHER_DATA_BUFFER_LENGTH)
  {
    memcpy(sDataBuffer, buffer, length);
    sDataBufferSize = length;
    result = 1;
  }
  else
  {
    sDataBufferSize = 0;
    result = 0;
  }
  return result;
}

const char * Dispatcher_GetDataBuffer(void)
{
  return sDataBuffer;
}

unsigned int Dispatcher_GetDataBufferSize(void)
{
  return sDataBufferSize;
}

tCommand Dispatcher_Wait(void)
{
  tCommand command;

  xQueueReceive(sQueueHandle, &command, portMAX_DELAY);

  return command;
}
