#include "nserver.h"
#include "esp_camera.h"

/* 1. Define the handler function */
esp_err_t hello_get_handler(httpd_req_t *req) {
    const char* resp_str = "<h1>Hello from ESP32!</h1><p>Dead simple, right?</p>";
    httpd_resp_send(req, resp_str, strlen(resp_str));
    return ESP_OK;
}

esp_err_t stream_handler(httpd_req_t *req) {
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    char * part_buf[64];

    // Set the main container header
    httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);

    while(1) {
        fb = esp_camera_fb_get(); // Capture a frame
        if (!fb) {
            return ESP_FAIL;
        }

        // Send the boundary and the part header (image size)
        size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, fb->len);
        res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);

        // Send the actual JPEG data
        if(res == ESP_OK) {
            res = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
        }

        // Send the delimiter
        if(res == ESP_OK) {
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }

        esp_camera_fb_return(fb); // Return frame buffer to be reused
        if(res != ESP_OK) break;
    }

    return res;
}


httpd_handle_t start_webserver(void) {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    /* 2. Map the URI to the handler */
    httpd_uri_t hello_uri = {
        .uri       = "/hello",
        .method    = HTTP_GET,
        .handler   = hello_get_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t stream_uri = {
        .uri       = "/stream",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };

    // Inside your start_webserver function:
    httpd_register_uri_handler(server, &stream_uri);

    /* 3. Start the server */
    if (httpd_start(&server, &config) == ESP_OK) {
        // Register URI handlers
        httpd_register_uri_handler(server, &hello_uri);
        httpd_register_uri_handler(server, &stream_uri);
        return server;
    }
    return NULL;
}
