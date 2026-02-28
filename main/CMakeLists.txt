idf_component_register(
    SRCS "main.c" "nwifi.c" "ncamera.c"
    INCLUDE_DIRS "."
    REQUIRES esp32-camera
    REQUIRES esp_driver_gpio
    PRIV_REQUIRES nvs_flash esp_wifi esp_event
)
