
#include "control-task/control-task.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include "pwm/pwm.h"
#include "drivers/i2c/i2c.h"
#include "drivers/rtc/rtc.h"
#include "drivers/gpio/gpio.h"
#include "stm32f103xb.h"

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
	/* Initialized first - do not move it! */
	Pwm_Initialize();

	/* No sub-priorities - FreeRTOS requirement */
	NVIC_SetPriorityGrouping(0);
	/* Queue initialization must be performed before
	 * RTC and GPIO external interrupts */
	Control_InitializeQueue();
	I2C_Initialize();
	Rtc_Initialize();
	Gpio_Initialize();
}

