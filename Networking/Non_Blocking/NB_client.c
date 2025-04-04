#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    // Server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Non-blocking connect
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        if (errno != EINPROGRESS) {
            perror("Connect failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("Connecting to server...\n");
    }

    while (1) {
        printf("Enter message (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting...\n");
            send(sockfd, buffer, strlen(buffer), 0);
            break;
        }

        int bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
        if (bytes_sent < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("Socket not ready for sending yet\n");
                continue;
            } else {
                perror("Send failed");
                break;
            }
        }

        usleep(100000);  

        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("No response yet\n");
            } else {
                perror("Receive failed");
                break;
            }
        } else {
            buffer[bytes_received] = '\0';
            printf("Echoed back: %s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}
