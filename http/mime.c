#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mime.h"

#define DEFAULT_MIME "application/octet-stream"

char *get_mime_type (char *buf) {
    char *extension = strrchr (buf, '.');

    if (extension) {
        extension++;
        char *p = extension;
        while (*p) {
            *p = tolower(*p);
            p++;
        }
        fprintf(stdout, "file extension of %s is %s\n", buf, extension);

    }

    if (strcmp(extension, "html") == 0 || strcmp(extension, "htm") == 0) {
        return "text/html";
    } else if (strcmp(extension, "js") == 0) {
        return "application/javascript";
    } else if (strcmp(extension, "jpeg") == 0 || strcmp(extension, "jpg") == 0) {
        return "image/jpg";
    } else if (strcmp(extension, "css") == 0) {
        return "text/css";
    } else if (strcmp(extension, "json") == 0) {
        return "application/json";
    } else if (strcmp(extension, "gif") == 0) {
        return "image/gif";
    } else if (strcmp(extension, "png") == 0) {
        return "image/png";
    } else if (strcmp(extension, "txt") == 0) {
        return "text/plain";
    } else {
        return DEFAULT_MIME;
    }

    return DEFAULT_MIME;
}   
