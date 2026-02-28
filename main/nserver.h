#ifndef NSERVER_H
#define NSERVER_H

#include "esp_http_server.h"
#include "esp_event.h"


#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

esp_err_t hello_get_handler(httpd_req_t *req);
httpd_handle_t start_webserver(void);

#endif
