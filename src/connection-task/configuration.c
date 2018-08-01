/*
 configuration.c

  Created on: Aug 1, 2018
      Author: Rafał Olejniczak
 */

#include "connection-task/configuration.h"
#include "stm32f10x_flash.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

#define CONFIG_ADDRESS  (0x0801FC00)

#define CONFIG_MAGIC  (0xDEADBEEFu)

static const tConfiguration * sSavedSettings =
    (const tConfiguration *) CONFIG_ADDRESS;

tConfiguration gConfigLocal;

void Configuration_Init(void)
{
  FLASH_SetLatency(FLASH_Latency_2);
}

int Configuration_Get(tConfiguration * config)
{
  int result;

  if (sSavedSettings->magic == CONFIG_MAGIC)
  {
    memcpy(config, sSavedSettings, sizeof(tConfiguration));
    result = 1;
  }
  else
  {
    result = 0;
  }

  return result;
}

void Configuration_Set(tConfiguration * config)
{
  unsigned int counter = sizeof(tConfiguration)/sizeof(uint16_t);
  const uint16_t * data = (const uint16_t *) config;
  uint32_t addr = CONFIG_ADDRESS;

  configASSERT(sizeof(tConfiguration) <= 1024);

  config->magic = CONFIG_MAGIC;

  FLASH_Unlock();

  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

  FLASH_ErasePage(CONFIG_ADDRESS);

  while (counter--)
  {
    FLASH_ProgramHalfWord(addr, *data);
    addr += 2;
    ++data;
  }

  FLASH_Lock();
}
