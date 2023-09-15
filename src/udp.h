#ifndef UDP_H_
#define UDP_H_

int create_udp_listener(int port);
int udp_echo(int socket, char* buf, int buflen);

#endif // UDP_H_
