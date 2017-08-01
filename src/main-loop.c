#include "stm32f103xb.h"

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
	__asm__("nop");
}

