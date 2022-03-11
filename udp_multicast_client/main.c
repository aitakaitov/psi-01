#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {

    char* group;
    int port;
    if (argc == 1) {
        group = "239.255.255.250";
        port = 1900;
        printf("No group or ports provided, using defaults (addr: %s, port: %d)\n", group, port);
    }
    else if (argc == 3) {
        group = argv[1];
        port = atoi(argv[2]);
    }
    else {
        printf("Usage: udp_multicast_client <multicast group> <port>\n");
        printf("or\n");
        printf("udp_multicast_client (this uses defaults)\n");
        return 1;
    }

    // Create UDP socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Set up address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    // Listen on all interfaces
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    // Bind
    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("Error binding");
        return 1;
    }

    // Join the multicast group
    struct ip_mreq mreq;
    // Set the multicast address to the group
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    // Set interface to any
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    // Request kernel to join the group
    if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0) {
        perror("Error setting multicast group");
        return 1;
    }

    puts("Socket bound and group registered, listening for multicasts");

    // now just enter a read-print loop
    char buffer[BUFFER_SIZE];
    for (;;) {
        u_int addrlen = sizeof(addr);
        size_t bytes_received = recvfrom(
                fd,
                buffer,
                BUFFER_SIZE,
                0,
                (struct sockaddr *) &addr,
                &addrlen
        );
        if (bytes_received < 0) {
            perror("Error reading from socket");
            return 1;
        }
        // NULL-terminate the buffer
        buffer[bytes_received] = '\0';
        puts(buffer);
    }
}
