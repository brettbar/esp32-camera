#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <esp_system.h>
#include "esp_log.h"
#include "esp_camera.h"

#include "driver/gpio.h"

#include "camera_pins.h"

#define BLINK_LED 21



void startCameraServer();

void app_main(void) {
    char *our_task_name = pcTaskGetName(NULL);

    camera_config_t camera_config = {
        .pin_pwdn       = PWDN_GPIO_NUM,
        .pin_reset      = RESET_GPIO_NUM,
        .pin_xclk       = XCLK_GPIO_NUM,
        .pin_sccb_sda   = SIOD_GPIO_NUM,
        .pin_sccb_scl   = SIOC_GPIO_NUM,
        .pin_d7         = Y9_GPIO_NUM,
        .pin_d6         = Y8_GPIO_NUM,
        .pin_d5         = Y7_GPIO_NUM,
        .pin_d4         = Y6_GPIO_NUM,
        .pin_d3         = Y5_GPIO_NUM,
        .pin_d2         = Y4_GPIO_NUM,
        .pin_d1         = Y3_GPIO_NUM,
        .pin_d0         = Y2_GPIO_NUM,
        .pin_vsync      = VSYNC_GPIO_NUM,
        .pin_href       = HREF_GPIO_NUM,
        .pin_pclk       = PCLK_GPIO_NUM,

        .xclk_freq_hz   = 20000000, // The clock frequency of the image sensor
        .fb_location    = CAMERA_FB_IN_PSRAM, // Set the frame buffer storage location
        /* .fb_location = CAMERA_FB_IN_DRAM, */
        .pixel_format   = PIXFORMAT_JPEG, // The pixel format of the image: PIXFORMAT_ + YUV422|GRAYSCALE|RGB565|JPEG
        .frame_size     = FRAMESIZE_UXGA, // The resolution size of the image: FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
        .jpeg_quality   = 12, // The quality of the JPEG image, ranging from 0 to 63.
        .fb_count       = 2, // The number of frame buffers to use.
        .grab_mode      = CAMERA_GRAB_WHEN_EMPTY //  The image capture mode.
    };


    esp_err_t err = esp_camera_init(&camera_config);
    if ( err != ESP_OK ) {
        ESP_LOGI(our_task_name, "Error encountered with esp camera init\n");
        return;
    }

    gpio_reset_pin(BLINK_LED);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(BLINK_LED, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_LED, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/* void app_main(void) { */
/*     char *our_task_name = pcTaskGetName(NULL); */
/**/
/*     ESP_LOGI(our_task_name, "Hello, starting up\n"); */
/**/
/*     gpio_reset_pin(BLINK_LED); */
/*     gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT); */
/**/
/*     while (1) { */
/*         gpio_set_level(BLINK_LED, 1); */
/*         vTaskDelay(1000 / portTICK_PERIOD_MS); */
/*         gpio_set_level(BLINK_LED, 0); */
/*         vTaskDelay(1000 / portTICK_PERIOD_MS); */
/*     } */
/* } */
