/*
 pages.h

  Created on: Mar 25, 2018
      Author: Rafał Olejniczak
 */

#ifndef INC_RESOURCES_PAGES_H_
#define INC_RESOURCES_PAGES_H_

#include "uchttpserver.h"

extern tHttpStatusCode AccessPointsCallback(void * const conn);
extern tHttpStatusCode ConnectCallback(void * const conn);
extern tHttpStatusCode DisconnectCallback(void * const conn);
extern tHttpStatusCode FaviconCallback(void * const conn);
extern tHttpStatusCode IndexCallback(void * const conn);
extern tHttpStatusCode ScriptCallback(void * const conn);
extern tHttpStatusCode StatusCallback(void * const conn);
extern tHttpStatusCode StyleCallback(void * const conn);

extern void OnError(
    void *const conn,
    const tErrorInfo *errorInfo);

#endif /* INC_GENERATED_PAGES_H_ */
