/*
 * app_main.c
 *
 *  Created on: Sep 29, 2025
 *      Author: kar-a
 */

/* This is the application main.
 * This is called from the Core/main.c */

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "app_main_config.h"
#include "remote_control_config.h"

/* Globals */
QueueHandle_t remote_sig_queue;



void app_main_resources(void) {


}

void app_main(void) {

	remote_control();
	vTaskStartScheduler();
}

