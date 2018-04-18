/*
 * resources.c
 *
 *  Created on: Mar 11, 2018
 *      Author: Rafał Olejniczak
 */

#include "port/uchttpserver/resources.h"
#include "resources/pages.h"

const tResourceEntry resources[] =
    {
	{ STRING_WITH_LENGTH("/accesspoints.json"), &AccessPointsCallback },
	{ STRING_WITH_LENGTH("/connect.html"), &ConnectCallback },
	{ STRING_WITH_LENGTH("/disconnect.html"), &DisconnectCallback },
	{ STRING_WITH_LENGTH("/favicon.ico"), &FaviconCallback },
	{ STRING_WITH_LENGTH("/index.html"), &IndexCallback },
	{ STRING_WITH_LENGTH("/script.js"), &ScriptCallback },
	{ STRING_WITH_LENGTH("/status.html"), &StatusCallback },
	{ STRING_WITH_LENGTH("/style.css"), &StyleCallback }
    };
