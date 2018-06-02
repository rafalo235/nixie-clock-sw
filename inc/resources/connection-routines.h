/*
 connection-routines.h

  Created on: Jun 1, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_RESOURCES_CONNECTION_ROUTINES_H_
#define INC_RESOURCES_CONNECTION_ROUTINES_H_

#include "resources/routine.h"

#define CONN_CRED_APN_LEN 32
#define CONN_CRED_PAS_LEN 32

typedef enum ConnectionRoutinesResults
{
  /* Connect results */
  CONN_ROUTINE_WIFI_CONNECTED,
  CONN_ROUTINE_WIFI_WRONG_CREDENTIALS,
  CONN_ROUTINE_WIFI_CONNECTION_ERROR,
  CONN_ROUTINE_WIFI_ALREADY_CONNECTED,

  /* Diconnection results */
  CONN_ROUTINE_WIFI_DISCONNECTED,
  CONN_ROUTINE_WIFI_NOT_CONNECTED,
  CONN_ROUTINE_WIFI_DISCONNECTION_ERROR,

  /* SNTP setup */
  CONN_ROUTINE_SNTP_CONFIGURED,
  CONN_ROUTINE_SNTP_CONFIG_ERROR
} tConnectionRoutinesResults;

typedef struct ConnectionCredentials
{
  char apn[CONN_CRED_APN_LEN];
  char password[CONN_CRED_PAS_LEN];
} tConnectionCredentials;

extern tRoutine gConnectionRoutine;

const char * ResolveResultMessage(tConnectionRoutinesResults result);

int ConnectToAccessPoint(void * param);

int DisconnectFromAccessPoint(void * param);

int SetSNTPConfig(void * param);

#endif /* INC_RESOURCES_CONNECTION_ROUTINES_H_ */
