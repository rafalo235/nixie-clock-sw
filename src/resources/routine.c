/*
 routine.c

  Created on: May 31, 2018
      Author: Rafał Olejniczak
 */

#include "resources/routine.h"

tRoutineStatus Routine_CallRoutine(
    tRoutine * routine, tRoutineFunction func, void *param)
{
  tRoutineStatus status;

  if (ROUTINE_NOT_CALLED == routine->status)
  {
    routine->func = func;
    routine->param = param;
    routine->status = ROUTINE_ONGOING;
    status = ROUTINE_ONGOING;
  }
  else
  {
    status = ROUTINE_ERROR;
  }

  return status;
}

tRoutineStatus Routine_ExecuteRoutine(
    tRoutine * routine)
{
  tRoutineStatus status;

  if (ROUTINE_ONGOING == routine->status && routine->func)
  {
    routine->result = routine->func(routine->param);
    routine->status = ROUTINE_FINISHED;
    status = ROUTINE_FINISHED;
  }
  else
  {
    status = ROUTINE_NOT_CALLED;
  }

  return status;
}

tRoutineStatus Routine_GetRoutineResult(
    tRoutine * routine, int * result)
{
  tRoutineStatus status = routine->status;

  if (ROUTINE_FINISHED == routine->status)
  {
    *result = routine->result;
    routine->status = ROUTINE_NOT_CALLED;
  }

  return status;
}

