/*
 pages.h

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_RESOURCES_PAGES_H_
#define INC_RESOURCES_PAGES_H_

#include "uchttpserver.h"

extern tHttpStatusCode IndexCallback(void * const conn);
extern tHttpStatusCode StyleCallback(void * const conn);
extern tHttpStatusCode ScriptCallback(void * const conn);

#endif /* INC_GENERATED_PAGES_H_ */
