#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

// Create a listening socket
int create_udp_listener(int port) {
    int listen_fd;
    struct sockaddr_in serveraddr;

    listen_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listen_fd < 0) {
        return -1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_fd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
        return -1;
    }

    return listen_fd;
}
