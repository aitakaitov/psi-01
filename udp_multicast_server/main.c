#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: <multicast group> <port>\n");
        return 1;
    }

    char* group = argv[1];
    int port = atoi(argv[2]);
    const char *message = "This is a UDP multicast message!";
    const int delay_secs = 5;

    // Create UDP socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("Error creating socket");
        return 1;
    }

    // Address setup
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    // Listen on the group address
    addr.sin_addr.s_addr = inet_addr(group);
    addr.sin_port = htons(port);

    puts("Server is up and running");

    for (;;) {
        size_t bytes_sent = sendto(
                fd,
                message,
                strlen(message),
                0,
                (struct sockaddr*) &addr,
                sizeof(addr)
        );

        if (bytes_sent < 0) {
            perror("Error sending data to multicast group");
            return 1;
        }

        sleep(delay_secs); // Unix sleep is seconds
    }
}