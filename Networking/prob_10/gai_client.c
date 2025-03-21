// using getaddrinfo client code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "3490"  // Port to connect to
#define MAXDATASIZE 100  // Buffer size for receiving data

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res;
    int sockfd;
    char buf[MAXDATASIZE];
    int bytes_received;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    // Setup hints structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    // Get address info
    if (getaddrinfo(argv[1], PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    // Create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Connect to server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }
    freeaddrinfo(res);

    printf("Connected to server. Waiting for message...\n");

    // Receive data
    bytes_received = recv(sockfd, buf, MAXDATASIZE - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
    } else {
        buf[bytes_received] = '\0';  // Null-terminate received data
        printf("Message from server: %s\n", buf);
    }

    // Close socket
    close(sockfd);
    return 0;
}
