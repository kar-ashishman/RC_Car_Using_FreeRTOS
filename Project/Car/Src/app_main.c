#include "FreeRTOS.h"
#include "Task.h"

void test(void *pvParameters) {
	while(1);
}


void app_main(void) {
	xTaskCreate(test, NULL, 100, NULL, 1, NULL);
	vTaskStartScheduler();
}
