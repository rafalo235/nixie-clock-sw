
#include "pwm/pwm.h"
#include "drivers/i2c/i2c.h"
#include "drivers/rtc/rtc.h"
#include "time/time.h"
#include "utils/coding.h"
#include "drivers/gpio/gpio.h"
#include "FreeRTOS.h"
#include "task.h"

void SystemInit(void);

void main(void)
{
	uint32_t tmp = 0, prev = 0;
	uint8_t hours = 0, minutes = 0;
#define SWAP_N_BITS(x,n)	(((x) << (n)) | ((x) >> (n)))

	while (Rtc_Read(&tmp) == RTC_SUCCESS)
	{
		if (prev != tmp)
		{
			tTime_DateTime t;
			t = Time_GetUTCTime(tmp, &t);

			t.minute = Utils_BinToBcd(t.minute);
			t.hour = Utils_BinToBcd(t.hour);

			minutes = SWAP_N_BITS(t.minute, 4);
			hours = SWAP_N_BITS(t.hour, 4);

			I2C_Write(0x70, &minutes, 1);
			I2C_Write(0x72, &hours, 1);

			prev = tmp;
		}
	}


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

