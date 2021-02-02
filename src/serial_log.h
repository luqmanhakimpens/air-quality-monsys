/*
 * serial_log.h
 *
 *  Created on: Jul 23, 2019
 *      Author: luqma
 */

#ifndef SERIAL_DEBUG_H_
#define SERIAL_DEBUG_H_

#include <app_config.h>

#ifdef LOG_ENABLE
	#define LOG_PRINTLN(...)	LOG_PORT.println(__VA_ARGS__)
	#define LOG_PRINT(...)		LOG_PORT.print(__VA_ARGS__)
	#define LOG_PRINTF(...)		LOG_PORT.printf(__VA_ARGS__)
#else
	#define LOG_PRINTLN(str)	{}
	#define LOG_PRINT(str)	{}
#endif




#endif /* LIB_SERIAL_LOG_SERIAL_LOG_H_ */
