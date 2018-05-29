/*
 common.h

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_RESOURCES_COMMON_H_
#define INC_RESOURCES_COMMON_H_

#include "esp8266.h"
#include "uchttpserver.h"

extern volatile ESP_t sEsp;

tuCHttpServerState *
GetServer(ESP_CONN_t * ctx, int conn);
void ReleaseServer(int conn);
void Disconnect(volatile ESP_t* ESP, ESP_CONN_t* conn);

int Connection_IsConnected(void);
void Connection_SetConnected(int connected);

void Page_SendInfoElement(
    void * const conn, const char * label, const char * value);

void Page_SendButton(
    void * const conn, const char * display, const char * callback);
void Page_SendPasswordPopup(
    void * const conn, const char * modalid,
    const char * id, const char * onclick);
void Page_SendErrorPopup(
    void * const conn, const char * modalid, const char * message);

void Json_OpenObject(void * const conn);
void Json_CloseObject(void * const conn);

void Json_OpenArray(void * const conn);
void Json_CloseArray(void * const conn);

void Json_Attribute(void * const conn, const char * name);
void Json_Value(void * const conn, const char * value);
void Json_Separator(void * const conn);

#endif /* INC_RESOURCES_COMMON_H_ */
