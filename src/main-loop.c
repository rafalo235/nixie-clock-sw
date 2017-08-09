
#include "control-task/control-task.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include "pwm/pwm.h"
#include "drivers/i2c/i2c.h"
#include "drivers/rtc/rtc.h"
#include "drivers/gpio/gpio.h"

void SystemInit(void);

void main(void)
{
	/* TODO static task since v9.0.0 */
	xTaskCreate(Control_Task, "ControlTask", 256, NULL, 1, NULL);

	vTaskStartScheduler();

	while (1)
	{
		__asm__("nop");
	}
}

void SystemInit(void)
{
	Pwm_Initialize();
	I2C_Initialize();
	Rtc_Initialize();
	Gpio_Initialize();
}

