#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "utils.h"

#define BUFLEN 51
#define PORT 5300
#define MAX_EPOLL_EVENTS 10

int main(void) {
    struct sockaddr_in si_me, si_other;

    // create udp socket
    int s;
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        perror("unable to create socket");
        exit(1);
    }

    // initialize server socket address
    memset((char *)&si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind the socket to the port
    if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1){
        perror("unable to bind socket");
        exit(1);
    }

    // create epoll instance
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1){
        perror("unable to create epoll instance");
        exit(1);
    }

    struct epoll_event event, events[MAX_EPOLL_EVENTS];
    event.events = EPOLLIN; // we are interested in read events
    event.data.fd = s;      // the UDP socket file descriptor

    // register the UDP socket with epoll
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, s, &event ) == -1){
        perror("unable to add fd (s) to epoll");
        exit(1);
    }

    // main event loop
    while (1) {
        // wait indefinetly for an event
        int n = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);

        // handle events
        for (int i = 0; i < n; i++) {
            if (events[i].events & EPOLLIN) {
                // receive the UDP packet
                char buf[BUFLEN];
                int slen = sizeof(si_other);
                int recv_len;
                if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, (socklen_t*)&slen)) == -1){
                    perror("unable to receive data");
                    exit(1);
                }

                // parse and print DNS name query
                char hex_str[BUFLEN * 2 + 1];  // Enough space for the hex
                                               // string and null-terminator

                dns_query_to_hex((const unsigned char*)buf, hex_str, recv_len);
                printf("received DNS query (HEX): %s\n", hex_str);

                /* char table[2048]; */
                /* packet_debug_table_to_buffer((const unsigned char*)buf, recv_len, table, sizeof(table)); */
                /* printf("%s", table); */

                char rfcPacket[1024];
                format_packet_RFC_style((const unsigned char*)buf, recv_len, rfcPacket, sizeof(rfcPacket));
                printf("%s", rfcPacket);

                // reply back the same request
                // (struct sockaddr*)&si_other, slen
                if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, slen) == -1){
                    perror("unable to reply back to the client");
                    exit(1);
                }
            }
        }
    }
    close(s);
    return 0;
}
