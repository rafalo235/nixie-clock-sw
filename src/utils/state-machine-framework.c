/*
 * state-machine-framework.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Rafal Olejniczak
 */
#include "utils/state-machine-framework.h"

void Utils_ProcessStateMachine(tUtils_StateMachine *sm, tUtils_Signal sig)
{
	if (sm->current != sm->next)
	{
		/* Changing state in leave or enter is forbidden */
		sm->current(sm, UTILS_LEAVE_STATE);
		sm->next(sm, UTILS_ENTER_STATE);
		sm->current = sm->next;
	}
	sm->current(sm, sig);
}
