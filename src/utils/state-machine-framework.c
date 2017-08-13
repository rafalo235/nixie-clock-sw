/*
 * state-machine-framework.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Rafal Olejniczak
 */
#include "utils/state-machine-framework.h"

void Utils_ProcessStateMachine(void *ptr, tUtils_Signal sig)
{
	tUtilsStateMachine *sm = ptr;
	if (sm->current != sm->next)
	{
		sm->next(sm, UTILS_ENTER_STATE);
		sm->current = sm->next;
	}
	sm->current(sm, sig);
	if (sm->current != sm->next)
	{
		sm->current(sm, UTILS_LEAVE_STATE);
		sm->current = sm->next;
	}
}
