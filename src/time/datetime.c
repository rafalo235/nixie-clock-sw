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

static void Datetime_UpdateTime(
    tDatetime * dt, uint32_t * timestamp, int * carry);
static void Datetime_UpdateCalendar(tDatetime * dt);

static uint32_t Datetime_IncrementTimeUnit(
    uint32_t unit, uint32_t * inc, uint32_t step, int * carry);

static uint32_t Datetime_GetMonthDays(uint32_t month, int leap);
static int Datetime_IsLeapYear(uint32_t year);

void Datetime_Increment(tDatetime * dt, uint32_t timestamp)
{
  int carry = 0;

  Datetime_UpdateTime(dt, &timestamp, &carry);

  timestamp += carry;

  /* This loop take some time in case of long power off
   * it should be considered to move it outside ISR */
  while (timestamp--)
  {
    Datetime_UpdateCalendar(dt);
  }
}

static void Datetime_UpdateTime(
    tDatetime * dt, uint32_t * timestamp, int * carry)
{
  dt->seconds = Datetime_IncrementTimeUnit(
      dt->seconds, timestamp, SECONDS_IN_MINUTE, carry);

  dt->minutes = Datetime_IncrementTimeUnit(
      dt->minutes, timestamp, MINUTES_IN_HOUR, carry);

  dt->hours = Datetime_IncrementTimeUnit(
      dt->hours, timestamp, HOURS_IN_DAY, carry);
}

static void Datetime_UpdateCalendar(
    tDatetime * dt)
{
  int leap = Datetime_IsLeapYear(dt->year);
  uint32_t monthDays = Datetime_GetMonthDays(dt->month, leap);

  if (dt->day < (monthDays - 1u))
  {
    ++(dt->day);
  }
  else
  {
    dt->day = 0u;
    ++(dt->month);
    if (11u == dt->month)
    {
      ++(dt->year);
    }
  }
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
