#include "udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "epoll.h"

#define BUFF_SIZE 51

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
            char buf[BUFF_SIZE];
            if (udp_echo(fd, buf, BUFF_SIZE) == -1){
                perror("unable to echo UDP message");
                exit(1);
            }
        }
    }
}
