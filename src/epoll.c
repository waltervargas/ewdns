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
