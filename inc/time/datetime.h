/*
 datetime.h

  Created on: Jul 27, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_TIME_DATETIME_H_
#define INC_TIME_DATETIME_H_

#include "esp/esp.h"
#include "esp/esp_sntp.h"

typedef esp_datetime_t tDatetime;

void Datetime_Increment(tDatetime * dt, uint32_t timestamp);

void Datetime_IncrementTime(tDatetime * dt, uint32_t * timestamp, int * carry);

void Datetime_IncrementDay(tDatetime * dt);

void Datetime_IncrementMonth(tDatetime * dt);

void Datetime_IncrementYear(tDatetime * dt);

void Datetime_Decrement(tDatetime * dt, uint32_t timestamp);

void Datetime_DecrementTime(tDatetime * dt, uint32_t * timestamp, int * carry);

void Datetime_DecrementDay(tDatetime * dt);

void Datetime_DecrementMonth(tDatetime * dt);

void Datetime_DecrementYear(tDatetime * dt);

#endif /* INC_TIME_DATETIME_H_ */
