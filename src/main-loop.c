
#include "pwm/pwm.h"
#include "drivers/i2c/i2c.h"
#include "FreeRTOS.h"
#include "task.h"

void SystemInit(void);

void main(void)
{
	uint8_t hours = 0, minutes = 0;
#define SWAP_N_BITS(x,n)	(((x) << (n)) | ((x) >> (n)))

	minutes = SWAP_N_BITS(minutes, 4);
	hours = SWAP_N_BITS(hours, 4);

	I2C_Write(0x70, &minutes, 1);
	I2C_Write(0x72, &hours, 1);

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
}

