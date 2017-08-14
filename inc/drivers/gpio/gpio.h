/*
 * gpio.h
 *
 *  Created on: Aug 8, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_DRIVERS_GPIO_GPIO_H_
#define INC_DRIVERS_GPIO_GPIO_H_

void EXTI1_IRQHandler(void);
void EXTI3_IRQHandler(void);

void Gpio_Initialize(void);

#endif /* INC_DRIVERS_GPIO_GPIO_H_ */
