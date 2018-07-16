
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

#define STACK_SIZE_CONTROL_TASK     (128)
#define STACK_SIZE_CONNECTION_TASK  (256)

void SystemInit(void);

static StaticTask_t sControlTaskControl;
static StaticTask_t sConnectionTaskControl;
static StaticTask_t sIdleTaskControl;

static StackType_t sControlTaskStack[STACK_SIZE_CONTROL_TASK];
static StackType_t sConnectionTaskStack[STACK_SIZE_CONNECTION_TASK];
static StackType_t sIdleTaskStack[configMINIMAL_STACK_SIZE];

void main(void)
{
  xTaskCreateStatic(Control_Task, "ControlTask",
      STACK_SIZE_CONTROL_TASK, NULL, 1, sControlTaskStack,
      &sControlTaskControl);

  xTaskCreateStatic(Connection_Task, "ConnectionTask",
      STACK_SIZE_CONNECTION_TASK, NULL, 1, sConnectionTaskStack,
      &sConnectionTaskControl);

  vTaskStartScheduler ();

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

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
  asm volatile ("nop");
}

void vApplicationGetIdleTaskMemory(
    StaticTask_t **controlBuffer,
    StackType_t **stackBuffer,
    uint32_t *stackBufferSize)
{
  *controlBuffer = &sIdleTaskControl;
  *stackBuffer = sIdleTaskStack;
  *stackBufferSize = configMINIMAL_STACK_SIZE;
}
