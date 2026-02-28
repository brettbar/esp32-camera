#include "nwifi.h"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "nvs_flash.h"  // Wi-Fi settings are stored in NVS
#include "nvs_flash.h"

void setup_wifi(void) {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();  // Creates the Station interface

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = "REDACTED",
              .password = "REDACTED",
          },
  };

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());
}

void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id,
                   void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    // 1. WIFI_EVENT_STA_START: The radio is powered on. Now we can connect.
    esp_wifi_connect();
    ESP_LOGI(TAG, "Connecting to AP...");

  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    // 2. WIFI_EVENT_STA_DISCONNECTED: Connection lost or failed.
    if (s_retry_num < MAXIMUM_RETRY) {
      esp_wifi_connect();
      s_retry_num++;
      ESP_LOGI(TAG, "Retry to connect to the AP");
    } else {
      xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
    }
    ESP_LOGI(TAG, "Connect to the AP failed");

  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    // 3. IP_EVENT_STA_GOT_IP: DHCP has assigned us an address!
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
    ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
    s_retry_num = 0;
    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
  }
}

void register_handlers(void) {
  // Register for all Wi-Fi events
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));

  // Register for IP events (specifically when we get an IP)
  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));
}

void connect_to_wifi(void) {
  /* Initializing the event group */
  s_wifi_event_group = xEventGroupCreate();

  /* Wait here until either:
     - The WIFI_CONNECTED_BIT is set by the event handler (Success)
     - The WIFI_FAIL_BIT is set (Too many retries)
  */
  EventBits_t bits = xEventGroupWaitBits(
      s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
      pdFALSE,         // Don't clear on exit
      pdFALSE,         // Don't wait for both; either one will do
      portMAX_DELAY);  // Wait forever

  /* Check which bit was actually set */
  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(TAG, "Connected to AP! Now starting internet tasks...");
    // YOUR CODE HERE (e.g., start_mqtt_client())
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGI(TAG, "Failed to connect to SSID.");
  } else {
    ESP_LOGE(TAG, "UNEXPECTED EVENT");
  }
}

void disconnect_wifi(void) {
  vEventGroupDelete(s_wifi_event_group);
  esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                        event_handler);
  esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                        event_handler);
}
