#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"

#include "driver/gpio.h"

#include "nwifi.h"
#include "ncamera.h"
#include "nserver.h"

#define BLINK_LED 21

void app_main(void) {
    char *our_task_name = pcTaskGetName(NULL);
    ESP_LOGI(our_task_name, "Starting up...\n");

    esp_err_t err = init_camera();
    if ( err != ESP_OK ) {
        ESP_LOGI(our_task_name, "Error encountered with esp camera init\n");
        return;
    }


	esp_err_t status = WIFI_FAILURE;

    init_storage();

    // connect to wireless AP
	status = connect_wifi();
	if (WIFI_SUCCESS != status)
	{
		ESP_LOGI(TAG, "Failed to associate to AP, dying...");
		return;
	}

	start_webserver();



	/* status = connect_tcp_server(); */
	/* if (TCP_SUCCESS != status) */
	/* { */
	/* 	ESP_LOGI(TAG, "Failed to connect to remote server, dying..."); */
	/* 	return; */
	/* } */

    gpio_reset_pin(BLINK_LED);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(BLINK_LED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}
