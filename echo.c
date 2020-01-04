#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "net.h"
// 1 kB
#define BUFSIZE 1 << 10
#define BACKLOG 10

void print_usage(char **argv) {
    fprintf(stdout, "Usage: %s <port number>\n", argv[0]);
    exit(1);
}

int echo(int clientfd) {
    char buf[BUFSIZE] = {0};
    int r;

    while ( (r = read(clientfd, buf, BUFSIZE)) > 0) {
        printf("Client said %s\n", buf);
        write(clientfd, buf, r);
        bzero(buf, BUFSIZE);
    }
    return r;
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
        echo (clientfd);
    }
    return 0;
}
