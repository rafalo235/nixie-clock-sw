/*
 * state-machine-framework.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_UTILS_STATE_MACHINE_FRAMEWORK_H_
#define INC_UTILS_STATE_MACHINE_FRAMEWORK_H_

/* Forward declaration */
struct Utils_StateMachine;

typedef enum Utils_Signal
{
	UTILS_ENTER_STATE = 0,
	UTILS_LEAVE_STATE = 1,
	UTILS_USER_SIGNAL = 2
} tUtils_Signal;

typedef void (*tUtils_State)(
		struct Utils_StateMachine *, tUtils_Signal);

typedef struct Utils_StateMachine
{
	tUtils_State current;
	tUtils_State next;
} tUtils_StateMachine;

void Utils_ProcessStateMachine(tUtils_StateMachine *, tUtils_Signal);

#endif /* INC_UTILS_STATE_MACHINE_FRAMEWORK_H_ */
