
#include "pwm/pwm.h"

void SystemInit(void);

int main(void)
{
	while (1)
	{
		__asm__("nop");
	}
}

void SystemInit(void)
{
	Pwm_Initialize();
}

