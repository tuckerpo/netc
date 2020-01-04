#pragma once
#ifndef __HTTP_H
#define __HTTP_H

typedef struct {
    char *method;
    char *uri;
    char *qs;
    char *protocol;
} http_request;

void http_request_free (http_request *req);

void http_request_free (http_request *req);
char *http_parse_method(const char* message);
char *http_parse_uri(const char* message);

http_request *http_request_new (const char *message); 

void handle_http(int fd); 


#endif // __HTTP_H
