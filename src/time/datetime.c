/*
 datetime.c

  Created on: Jul 27, 2018
      Author: Rafał Olejniczak
 */

#include "time/datetime.h"

#define IS_DIVISIBLE_UINT(x, y)  (0u == ((x) % (y)))

#define SECONDS_IN_MINUTE 60
#define MINUTES_IN_HOUR   60
#define HOURS_IN_DAY      24

static uint32_t Datetime_IncrementTimeUnit(
    uint32_t unit, uint32_t * inc, uint32_t step, int * carry);
static uint32_t Datetime_DecrementTimeUnit(
    uint32_t unit, uint32_t * dec, uint32_t step, int * carry);

static uint32_t Datetime_GetMonthDays(uint32_t month, int leap);
static int Datetime_IsLeapYear(uint32_t year);

void Datetime_Increment(tDatetime * dt, uint32_t timestamp)
{
  int carry = 0;

  Datetime_IncrementTime(dt, &timestamp, &carry);

  timestamp += carry;

  /* This loop take some time in case of long power off
   * it should be considered to move it outside ISR */
  while (timestamp--)
  {
    Datetime_IncrementDay(dt);
  }
}

void Datetime_Decrement(tDatetime * dt, uint32_t timestamp)
{
  int carry = 0;

  Datetime_DecrementTime(dt, &timestamp, &carry);

  timestamp += carry;

  while (timestamp--)
  {
    Datetime_DecrementDay(dt);
  }
}

void Datetime_IncrementTime(
    tDatetime * dt, uint32_t * timestamp, int * carry)
{
  dt->seconds = Datetime_IncrementTimeUnit(
      dt->seconds, timestamp, SECONDS_IN_MINUTE, carry);

  dt->minutes = Datetime_IncrementTimeUnit(
      dt->minutes, timestamp, MINUTES_IN_HOUR, carry);

  dt->hours = Datetime_IncrementTimeUnit(
      dt->hours, timestamp, HOURS_IN_DAY, carry);
}

void Datetime_DecrementTime(
    tDatetime * dt, uint32_t * timestamp, int * carry)
{
  dt->seconds = Datetime_DecrementTimeUnit(
      dt->seconds, timestamp, SECONDS_IN_MINUTE, carry);

  dt->minutes = Datetime_DecrementTimeUnit(
      dt->minutes, timestamp, MINUTES_IN_HOUR, carry);

  dt->hours = Datetime_DecrementTimeUnit(
      dt->hours, timestamp, HOURS_IN_DAY, carry);
}

void Datetime_IncrementDay(tDatetime * dt)
{
  int leap = Datetime_IsLeapYear(dt->year);
  uint32_t monthDays = Datetime_GetMonthDays(dt->month, leap);

  if (dt->date < (monthDays - 1u))
  {
    ++(dt->date);
  }
  else
  {
    dt->date = 0u;
    Datetime_IncrementMonth(dt);
  }
}

void Datetime_IncrementMonth(tDatetime * dt)
{
  ++(dt->month);
  if (12u == dt->month)
  {
    dt->month = 0u;
    Datetime_IncrementYear(dt);
  }
}

void Datetime_IncrementYear(tDatetime * dt)
{
  ++(dt->year);
}

void Datetime_DecrementDay(tDatetime * dt)
{
  int leap = Datetime_IsLeapYear(dt->year);

  if (dt->date > 0u)
  {
    --(dt->date);
  }
  else
  {
    Datetime_DecrementMonth(dt);
    dt->date = Datetime_GetMonthDays(dt->month, leap) - 1;
  }
}

void Datetime_DecrementMonth(tDatetime * dt)
{
  if (dt->month > 0u)
  {
    --(dt->month);
  }
  else
  {
    dt->month = 11u;
    Datetime_DecrementYear(dt);
  }
}

void Datetime_DecrementYear(tDatetime * dt)
{
  --(dt->year);
}

static uint32_t Datetime_IncrementTimeUnit(
    uint32_t unit, uint32_t * inc, uint32_t step, int * carry)
{
  unit += (*inc % step) + (*carry);
  *inc /= step;
  if (unit >= step)
  {
    *carry = 1;
    unit %= step;
  }
  else
  {
    *carry = 0;
  }
  return unit;
}

static uint32_t Datetime_DecrementTimeUnit(
    uint32_t unit, uint32_t * dec, uint32_t step, int * carry)
{
  uint32_t delta = ((*dec % step) + *carry);

  if (unit < delta)
  {
    unit = step - (delta - unit);
    *carry = 1;
  }
  else
  {
    unit -= delta;
    *carry = 0;
  }

  *dec /= step;
}

static uint32_t Datetime_GetMonthDays(uint32_t month, int leap)
{
  uint32_t days;
  uint32_t odd = month % 2u;

  if (1u == month)
  {
    if (0 == leap)
    {
      days = 28u;
    }
    else
    {
      days = 29u;
    }
  }
  else if (!odd && month < 7)
  {
    days = 31u;
  }
  else if (odd && month >= 7)
  {
    days = 31u;
  }
  else
  {
    days = 30u;
  }
  return days;
}

static int Datetime_IsLeapYear(uint32_t year)
{
  int leap;

  if (IS_DIVISIBLE_UINT(year, 400u))
  {
    leap = 1;
  }
  else if (IS_DIVISIBLE_UINT(year, 100u))
  {
    leap = 0;
  }
  else if (IS_DIVISIBLE_UINT(year, 4u))
  {
    leap = 1;
  }
  else
  {
    leap = 0;
  }

  return leap;
}
