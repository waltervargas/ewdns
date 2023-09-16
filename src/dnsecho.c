#include "udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

int epoll_setup_in(const int socket){
    int epoll_fd;

    // create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        return epoll_fd;
    }

    struct epoll_event event;
    event.events =  EPOLLIN; // we are interested in read events
    event.data.fd = socket;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, &event) == -1) {
        return -1;
    }
    return epoll_fd;
}


int main(void){
    int fd = create_udp_listener(5301);
    struct epoll_event events[10];

    int epoll_fd = epoll_setup_in(fd);

    while (1) {
        // wait indefinetly for an event
        // this call blocks
        int n = epoll_wait(epoll_fd, events, 10, -1);

        // handle events
        for (int i = 0; i < n; i++) {
            char buf[51];
            if (udp_echo(fd, buf, 51) == -1){
                perror("unable to echo UDP message");
                exit(1);
            }
        }
    }
}
