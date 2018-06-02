/*
 routine.h

  Created on: May 31, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_RESOURCES_ROUTINE_H_
#define INC_RESOURCES_ROUTINE_H_

typedef enum RoutineStatus
{
  ROUTINE_FINISHED,
  ROUTINE_ONGOING,
  ROUTINE_NOT_CALLED,
  ROUTINE_ERROR
} tRoutineStatus;

typedef int (*tRoutineFunction)(void *);

typedef struct Routine
{
  tRoutineFunction func;
  void * param;
  int result;
  tRoutineStatus status;
} tRoutine;

void Routine_Init(tRoutine * routine);

tRoutineStatus Routine_CallRoutine(
    tRoutine * routine, tRoutineFunction func, void *param);

tRoutineStatus Routine_ExecuteRoutine(
    tRoutine * routine);

tRoutineStatus Routine_GetRoutineResult(
    tRoutine * routine, int * result);

#endif /* INC_RESOURCES_ROUTINE_H_ */
