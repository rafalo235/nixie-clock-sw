/*
 * state-machine-framework.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_UTILS_STATE_MACHINE_FRAMEWORK_H_
#define INC_UTILS_STATE_MACHINE_FRAMEWORK_H_

#define STATE_CHANGE(x, state)	((x)->sm.next = state)

/* Forward declaration */
struct UtilsStateMachine;

typedef enum Utils_Signal
{
	UTILS_ENTER_STATE = 0,
	UTILS_LEAVE_STATE = 1,
	UTILS_USER_SIGNAL = 2
} tUtils_Signal;

typedef void (*tUtilsState)(void *, tUtils_Signal);

typedef struct UtilsStateMachine
{
	tUtilsState current;
	tUtilsState next;
} tUtilsStateMachine;

void Utils_ProcessStateMachine(void *, tUtils_Signal);

#endif /* INC_UTILS_STATE_MACHINE_FRAMEWORK_H_ */
