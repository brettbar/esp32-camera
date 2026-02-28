#ifndef NWIFI_H
#define NWIFI_H

#include "esp_event.h"

/* FreeRTOS event group to signal when we are connected */
static EventGroupHandle_t s_wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi_station";
static int s_retry_num = 0;
#define MAXIMUM_RETRY  5

void setup_wifi(void);
void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                   void* event_data);

void register_handlers(void);
void connect_to_wifi(void);
void disconnect_wifi(void);

#endif
