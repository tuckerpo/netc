#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "net.h"

#define BACKLOG 10

/* Return a fd corresponding to a bound TCP socket at portno, -1 on error. */
int get_tcp_listener_sock (char* portno) {
    int ret = 0;
    int socketfd;
    struct addrinfo hints, *serverinfo, *p;
    int sockoptsyes = 1;

    /* If portno is valid and outside of priviledged port range... */
    if (atoi(portno) > 1024 && atoi(portno) <= (1 << 16) - 1) {
       
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        if ((ret = getaddrinfo(NULL, portno, &hints, &serverinfo)) != 0) {
            perror("getaddrinfo");
            return -1;
        }
    
        /* Try all network interefaces - use first available */
        for (p = serverinfo; p; p = p->ai_next) {
            if ((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                continue;
            }
    
            if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &sockoptsyes, sizeof(sockoptsyes)) == -1) {
                perror("setsockopt");
                close(socketfd);
                freeaddrinfo(serverinfo);
                return -1;
            }
    
            if (bind(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
                perror("bind");
                close (socketfd);
                return -1;
            }
    
            /* Got a bound socket. */
            break;
        }
    
        freeaddrinfo(serverinfo);
    
        if (listen(socketfd, BACKLOG) == -1) {
            perror("listen");
            close(socketfd);
            return -1;
        }
    
        return socketfd;
    }

}

