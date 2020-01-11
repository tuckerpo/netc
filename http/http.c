#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "net.h"
#include "http.h"
#include "mime.h"

// 1 kB
#define BUFSIZE 1 << 10

void http_request_line_free (http_request_line *req_line) {
    if (req_line) {
        free(req_line->uri);
        free(req_line->prot);
    }
}

void http_body_free (http_request_body *body) {
    if (body) {
        // do thing
    }
}

void http_request_free (http_request *req) {
    if (req) {
        http_request_line_free (req->request_line);
        http_body_free (req->body);
        free(req);
    }
}

void http_print_body (http_request_body *body) {
    if (body) {
        // do thing
    }
}

void http_print_request_line (http_request_line *req_line) {
    if (req_line) {
        fprintf(stdout, "Request-line METHOD: %s, URI: %s, PROTOCOL: %s\n",
                req_line->method, req_line->uri, req_line->prot);
    }
}

void http_print(http_request *req) {
    if (req) {
        http_print_request_line (req->request_line);
        http_print_body (req->body);
    }
}

char *http_parse_protocol(const char *message) {
    char *prot;
    char *ret = NULL;
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
    char *ret = NULL;
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
    char *method = NULL;
    if ((method = strstr(message, "GET")) != NULL) {
        method = "GET"; 
    } else if ((method = strstr(message, "POST")) != NULL) {
        method = "POST";
    } else {
        return method;
    }
    return method;
}

http_request_line* http_parse_reqline(const char *message) {
    http_request_line *hrl = (http_request_line *)
        malloc(sizeof(http_request_line));
    if (hrl) {
        char *method = http_parse_method(message);
        char *uri = http_parse_uri(message);
        char *prot = http_parse_protocol(message);
        if (method && uri && prot) {
           hrl->method = method;
           hrl->uri = uri;
           hrl->prot = prot;
           return hrl;
        }
    }
    return NULL;
}

void set_response_status_line (int fd, int response_code) {
    dprintf(fd, "HTTP/1.1 %d\n", response_code);
}

http_request *http_request_new (const char *message) {
    http_request *new_http_req;
    new_http_req = (http_request *)malloc(sizeof(http_request));
    if (new_http_req == NULL) {
        printf("new_http_req == NULL\n");
        return NULL;
    }

    http_request_line *req_line = malloc(sizeof(http_request_line));
    if (req_line) {
        req_line = http_parse_reqline(message);
        if (!req_line) {
            printf("http_parse_reqline(%s) returned NULL\n", message);
            return NULL;
        }
        new_http_req->request_line = req_line;
    }
    return new_http_req;
}

int serve_file(int fd, const char *path, const char *content_type) {
    int filefd;
    struct stat file_stat;
    stat(path, &file_stat);
    filefd = open(path, O_RDONLY);
    if (filefd < 0) {
        // TODO send 500
        perror("open");
        return -1;
    }

    set_response_status_line (fd, 200);
    dprintf(fd, "Content-Type: %s\n", content_type);
    dprintf(fd, "Content-Length: %d\n", file_stat.st_size);
    dprintf(fd, "\n");
    off_t offset = 0;
    while (offset < file_stat.st_size) {
        ssize_t n = sendfile (fd, filefd, &offset, 1 << 10);
        if (n < 0) {
            perror("sendfile");
            break;
        }
    }

    if (filefd >= 0) {
        close(filefd);
    }

    return 0;
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
        fprintf(stderr, "http_request_new(%s) failed\n", reqbuf);
        return;
    }

    http_print(req);
    if (req->request_line->method == "GET") {
        printf("GET request\n");
        char *path = req->request_line->uri;
        printf("path %s\n", path);
        char* mime_type = get_mime_type(req->request_line->uri);
        serve_file (fd, /*req->request_line->uri*/"./index.html", mime_type);
    }
    http_request_free (req);

}


