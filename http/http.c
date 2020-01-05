#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "net.h"
#include "http.h"
// 1 kB
#define BUFSIZE 1 << 10
#define BACKLOG 10
#define CRLF "\r\n"


void print_usage(char **argv) {
    fprintf(stdout, "Usage: %s <port number>\n", argv[0]);
    exit(1);
}

size_t filesize(const char *path) {
    struct stat s;
    stat (path, &s);
    return s.st_size;
}

void http_request_free (http_request *req) {
    if (req) {
        //if (req->method) free (req->method); // method not malloc'd
        if (req->uri) free (req->uri);
        if (req->protocol) free (req->protocol);
        free (req);
    }
}

void http_print(http_request *req) {
    if (req) {
        fprintf(stdout, "req->method %s, req->uri %s, req->protocol %s\n",
                req->method, req->uri, req->protocol);
    }
}

char *http_parse_protocol(const char *message) {
    char *prot;
    char *ret;
    if ((prot = strstr(message, "HTTP")) != NULL) {
        char *p = prot;
        while (!isspace(*p)) p++;
        ret = (char *)malloc ( sizeof(char) * (p - prot) + 1);
        if (ret) {
            strncpy(ret, prot, (p - prot));
            ret[(p - prot)] = '\0';
        }
    }
    return ret;
}

char *http_parse_uri(const char *message) {
    char *uri;
    char *ret;
    if ((uri = strstr(message, "/")) != NULL) {
        char *p = uri;
        while (!isspace(*p)) p++;
        ret = (char *)malloc(sizeof(char) * (p - uri) + 1);
        if (ret) {
            strncpy(ret, uri, (p - uri));
            ret[(p - uri)] = '\0';
        }
    }
    return ret;
}

char *http_parse_method(const char* message) {
    char *method;
    if ((method = strstr(message, "GET")) != NULL) {
        method = "GET"; 
    } else if ((method = strstr(message, "POST")) != NULL) {
        method = "POST";
    } else {
        return method;
    }
    return method;
}

http_request *http_request_new (const char *message) {
    http_request *new = NULL;
    new = (http_request *)malloc(sizeof(http_request));
    if (new) {
        new->method = http_parse_method(message);
        new->uri = http_parse_uri(message);
        new->protocol = http_parse_protocol(message);
    }
    return new;
}

void handle_http(int fd) {
    http_request *req = NULL;
    const int bufsize = 1 << 16;
    char reqbuf[bufsize];
    int n = recv (fd, reqbuf, bufsize, 0);
    if (n < 0) {
        perror("recv");
        return;
    }
    reqbuf[n] = '\0';

    req = http_request_new (reqbuf);

    if (!req) {
        fprintf(stdout, "http_request_new(%s) failed\n", reqbuf);
        return;
    }

    http_print(req);
    http_request_free (req);
}


