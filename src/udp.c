#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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

int udp_echo(int socket, char* buf, size_t buflen){
    struct sockaddr_in caddr; // client addres
    socklen_t caddrlen = sizeof(caddr);
    memset((char *)&caddr, 0, caddrlen);

    int recv_len;
    if ((recv_len = recvfrom(socket, buf, buflen, 0, (struct sockaddr *)&caddr, &caddrlen)) == -1) {
        return -1;
    }

    int sent_len;
    if ((sent_len = sendto(socket, buf, buflen, 0, (struct sockaddr*)&caddr, caddrlen)) == -1){
         return -1;
    }

    return sent_len;
}
