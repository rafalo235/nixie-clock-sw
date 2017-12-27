/*
 * connection-task.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Rafal Olejniczak
 */

#include "connection-task/connection-task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/usart/usart.h"

void Connection_Task(void *parameters)
{

	while (1)
	{
		Usart_Write("AT+REG?\r\n", 9);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

}
