/*
 * app_main_config.h
 *
 *  Created on: Sep 29, 2025
 *      Author: kar-a
 */

#ifndef APP_MAIN_CONFIG_H_
#define APP_MAIN_CONFIG_H_

	#include "FreeRTOSConfig.h"
	#include "FreeRTOS.h"

	/* function proto */
	void app_main(void);

	/* Externs */
	extern QueueHandle_t remote_sig_queue;


#endif /* APP_MAIN_CONFIG_H_ */
