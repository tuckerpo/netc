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
// 1 kB
#define BUFSIZE 1 << 10
#define BACKLOG 10
#define CRLF "\r\n"

typedef struct {
    char *method;
    char *uri;
    char *qs;
    char *protocol;
} http_request;

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
        if (req->method) free (req->method);
        if (req->uri) free (req->uri);
        if (req->qs) free (req->qs);
        if (req->protocol) free (req->protocol);
        free (req);
    }
}

void http_print(http_request *req) {
    if (req) {
        fprintf(stdout, "req->method %s, req->uri %s, req->qs %s, req->protocol %s\n",
                req->method, req->uri, req->qs, req->protocol);
    }
}

char *http_parse_method(const char* message) {
    return "GET";
}

char *http_parse_uri(const char* message) {
    return "/tuckerino";
}

http_request *http_request_new (const char *message) {
    http_request *new = NULL;
    new = (http_request *)malloc(sizeof(http_request));
    if (new) {
        new->method = http_parse_method(message);
        new->uri = http_parse_uri(message);
        new->qs = "Hello, this is QS";
        new->protocol = "TuckProtocol";
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

    printf("client %d message %s\n", fd, reqbuf);

    req = http_request_new (reqbuf);

    if (!req) {
        fprintf("http_request_new(%s) failed\n", reqbuf);
        return;
    }

    http_print(req);

    http_request_free (req);
}

int main (int argc, char ** argv) {
    int clientfd;
    char clientip[INET_ADDRSTRLEN];
    struct sockaddr_storage client_addr;
    
    if (argc < 2) {
        print_usage (argv);
    }

    int socket = get_tcp_listener_sock(argv[1]); 

    if (socket < 0) {
        fprintf(stderr, "get_tcp_listener_socket(%s) failed\n", argv[1]);
        return -1;
    }
    
    while (1) {
        socklen_t sin_size = sizeof (client_addr);

        clientfd = accept (socket, (struct sockaddr *) &client_addr, &sin_size); 
        /* Connection failed, keep on truckin` */
        if (clientfd < 0) {
            perror("accept");
            continue;
        }

        struct sockaddr_in *sin = (struct sockaddr_in *) &client_addr;

        uint8_t * ip = (uint8_t *)&sin->sin_addr.s_addr;

        fprintf(stdout, "Client connected from %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

        handle_http(clientfd);
    }
    return 0;
}
