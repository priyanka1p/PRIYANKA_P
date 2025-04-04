#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUF_SIZE 1048576  // 1 MB buffer

// Function to send all data reliably
int sendall(int sockfd, const char *buf, int *len) {
    int total = 0;
    int bytesleft = *len;
    int n;

    while (total < *len) {
        n = send(sockfd, buf + total, bytesleft, 0);
        if (n == -1) {
            perror("send");
            break;
        }
        total += n;
        bytesleft -= n;
        printf("Sent chunk: %d bytes (Total: %d bytes)\n", n, total);
    }

    *len = total; // return number actually sent
    return (n == -1) ? -1 : 0;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    int len;

    // Fill the buffer with 'A's
    memset(buffer, 'A', BUF_SIZE - 1);
    buffer[BUF_SIZE - 1] = '\0';  // Null-terminate for string

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server!\n");

    // Send large data using sendall
    len = strlen(buffer);
    if (sendall(sockfd, buffer, &len) == -1) {
        fprintf(stderr, "Failed to send all data.\n");
    } else {
        printf("Successfully sent %d bytes to server.\n", len);
    }

    close(sockfd);
    return 0;
}
