/*
 * states.h
 *
 *  Created on: Aug 15, 2017
 *      Author: Rafal Olejniczak
 */

#ifndef INC_CONTROL_TASK_STATES_STATES_H_
#define INC_CONTROL_TASK_STATES_STATES_H_

#include "utils/state-machine-framework.h"

void IdleState(void *sm, tUtils_Signal sig);

void SetTimeState(void *sm, tUtils_Signal sig);

void ShowIpState(void *sm, tUtils_Signal sig);

#endif /* INC_CONTROL_TASK_STATES_STATES_H_ */
