
#include "control-task/control-task.h"
#include "connection-task/connection-task.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include "pwm/pwm.h"
#include "drivers/i2c/i2c.h"
#include "drivers/rtc/rtc.h"
#include "drivers/gpio/gpio.h"
#include "drivers/usart/usart.h"
#include "drivers/clock-source/clock-source.h"
#include "stm32f103xb.h"
#include "esp8266_ll.h"

void SystemInit(void);

void main(void)
{
	/* TODO static task since v9.0.0 */
	xTaskCreate(Control_Task, "ControlTask", 512, NULL, 1, NULL);
	xTaskCreate(Connection_Task, "ConnectionTask", 512, NULL, 1, NULL);
	xTaskCreate(Receiver_Task, "ReceiverTask", 256, NULL, 1, NULL);
	xTaskCreate(Update_Task, "UpdateTask", 256, NULL, 1, NULL);

	vTaskStartScheduler();

	while (1)
	{
		__asm__("nop");
	}
}

void SystemInit(void)
{
	/* Initialized first - do not move it! */
	Pwm_InitializeOffState();
	Pll_Initialize();

	/* No sub-priorities - FreeRTOS requirement */
	NVIC_SetPriorityGrouping(0);
	/* Queue initialization must be performed before
	 * RTC and GPIO external interrupts */
	Control_InitializeQueue();
	I2C_Initialize();
	Rtc_Initialize();
	Gpio_Initialize();
	Usart_Initialize();
	Pwm_Initialize();
}

