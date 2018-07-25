/*
 command-dispatcher.h

 MIT License

 Copyright (c) 2018 Rafał Olejniczak

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

  Created on: Jul 24, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_CONNECTION_TASK_COMMAND_DISPATCHER_H_
#define INC_CONNECTION_TASK_COMMAND_DISPATCHER_H_

#include "esp/esp.h"

typedef enum CommandType
{
  COMMAND_START_SERVER,
  COMMAND_STOP_SERVER,
  COMMAND_CLOSE_SERVER_CONNECTION,
  COMMAND_HANDLE_REQUEST
}tCommandType;

typedef struct Command
{
  tCommandType type;
  void * parameter;
} tCommand;

typedef void (tCommandCallback)(tCommand * command);

void Dispatcher_Init(void);

void Dispatcher_Send(tCommandType command, void * parameter);

int Dispatcher_SetDataBuffer(const void * buffer, unsigned int length);

unsigned int Dispatcher_GetDataBufferSize(void);

const char * Dispatcher_GetDataBuffer(void);

tCommand Dispatcher_Wait(void);

#endif /* INC_CONNECTION_TASK_COMMAND_DISPATCHER_H_ */
