#pragma once
#ifndef __HTTP_H
#define __HTTP_H

typedef struct {
    char *method;
    char *uri;
    char *prot;
} http_request_line;

typedef struct {
    char *body;
} http_request_body;

typedef struct {
    http_request_line* request_line;
    http_request_body* body;
} http_request;

int serve_file(int fd, const char *path, const char *content_type);
char *http_parse_method(const char* message);
char *http_parse_uri(const char* message);
http_request *http_request_new (const char *message); 
void http_request_free (http_request *req);
void handle_http(int fd); 

#endif // __HTTP_H
