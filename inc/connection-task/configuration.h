/*
 configuration.h

  Created on: Aug 1, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_CONNECTION_TASK_CONFIGURATION_H_
#define INC_CONNECTION_TASK_CONFIGURATION_H_

#include <stdint.h>

#define CONFIG_APN_LENGTH       32
#define CONFIG_PASSWORD_LENGTH  32
#define CONFIG_SNTP_ADDR_LENGTH 32

#define CONFIG_SNTP_AV_SERVERS  3

typedef struct Configuration
{
  char apn[CONFIG_APN_LENGTH];
  char password[CONFIG_PASSWORD_LENGTH];
  char sntp[CONFIG_SNTP_AV_SERVERS][CONFIG_SNTP_ADDR_LENGTH];
  int8_t timezone;
  unsigned int magic;
} tConfiguration;

extern tConfiguration gConfigLocal;

void Configuration_Init(void);

int Configuration_Get(tConfiguration * config);

void Configuration_Set(const tConfiguration * config);

#endif /* INC_CONNECTION_TASK_CONFIGURATION_H_ */
