/*
 * port.h
 *
 *  Created on: Mar 11, 2018
 *      Author: Rafał Olejniczak
 */

#ifndef INC_PORT_UCHTTPSERVER_PORT_H_
#define INC_PORT_UCHTTPSERVER_PORT_H_

unsigned int Http_SendPort(
    void * const  conn, const char * data, unsigned int length);

#endif /* INC_PORT_UCHTTPSERVER_PORT_H_ */
