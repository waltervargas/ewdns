#ifndef EPOLL_H_
#define EPOLL_H_

// setup_epoll_in create an epoll instance listening on EPOLLIN events
// return epoll_fd to be used later with epoll_wait(2)
int epoll_setup_in(const int socket);

#endif // EPOLL_H_
