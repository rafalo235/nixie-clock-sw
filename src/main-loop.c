
#include "pwm/pwm.h"
#include "FreeRTOS.h"
#include "task.h"

void SystemInit(void);

void main(void)
{
	vTaskStartScheduler();

	while (1)
	{
		__asm__("nop");
	}
}

void SystemInit(void)
{
	Pwm_Initialize();
}

