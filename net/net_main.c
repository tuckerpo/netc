#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "net.h"

void print_usage (char **argv) {
    fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
    exit(1);
}

int main (int argc, char **argv) {
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
    }
    return 0;
}
