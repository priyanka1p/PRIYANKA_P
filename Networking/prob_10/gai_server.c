#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "3490"  // Port to listen on
#define BACKLOG 10    // Maximum pending connections

int main() {
    struct addrinfo hints, *res;
    int sockfd, new_fd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    char msg[] = "Hello from server!";
    int bytes_sent;

    // Setup hints structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;    // Use my IP automatically

    // Get address info
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    // Create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Bind socket
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sockfd);
        return 1;
    }
    freeaddrinfo(res);

    // Listen for connections
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        close(sockfd);
        return 1;
    }

    printf("Server listening on port %s...\n", PORT);

    // Accept a connection
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd == -1) {
        perror("accept");
        close(sockfd);
        return 1;
    }

    printf("Connection accepted. Sending message...\n");

    // Send message to client
    bytes_sent = send(new_fd, msg, strlen(msg), 0);
    if (bytes_sent == -1) {
        perror("send");
    } else {
        printf("Message sent to client: %s\n", msg);
    }

    // Close sockets
    close(new_fd);
    close(sockfd);
    return 0;
}
