/*
 common.h

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_RESOURCES_COMMON_H_
#define INC_RESOURCES_COMMON_H_

void Page_SendButton(
    void * const conn, const char * display, const char * callback);

void Json_OpenObject(void * const conn);
void Json_CloseObject(void * const conn);

void Json_OpenArray(void * const conn);
void Json_CloseArray(void * const conn);

void Json_Attribute(void * const conn, const char * name);
void Json_Value(void * const conn, const char * value);
void Json_Separator(void * const conn);

#endif /* INC_RESOURCES_COMMON_H_ */