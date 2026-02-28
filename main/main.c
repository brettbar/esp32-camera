#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"

#include "driver/gpio.h"

#include "nwifi.h"
#include "ncamera.h"

#define BLINK_LED 21

void app_main(void) {
    char *our_task_name = pcTaskGetName(NULL);
    ESP_LOGI(our_task_name, "Starting up...\n");

    esp_err_t err = init_camera();
    if ( err != ESP_OK ) {
        ESP_LOGI(our_task_name, "Error encountered with esp camera init\n");
        return;
    }

    /* setup_wifi(); */
    /* register_handlers(); */
    /* connect_to_wifi(); */


    gpio_reset_pin(BLINK_LED);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(BLINK_LED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}
