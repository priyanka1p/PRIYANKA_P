#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 5
#define BUF_SIZE 1048576  // 10 KB buffer


int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUF_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(1);
    }

    // Listen for connections
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
        close(server_fd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        exit(1);
    }

    printf("Client connected!\n");

    // Receive large data in multiple parts
    int total_received = 0;
    int bytes_received;
    while ((bytes_received = recv(client_fd, buffer, BUF_SIZE, 0)) > 0) {
        total_received += bytes_received;
        printf("Received chunk: %d bytes (Total: %d bytes)\n", bytes_received, total_received);
    }

    if (bytes_received == -1) {
        perror("recv");
    } else {
        printf("Complete message received: %d bytes\n", total_received);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

Server listening on port 8080...
Client connected!
Received chunk: 65482 bytes (Total: 65482 bytes)
Received chunk: 32741 bytes (Total: 98223 bytes)
Received chunk: 98223 bytes (Total: 196446 bytes)
Received chunk: 852129 bytes (Total: 1048575 bytes)
Complete message received: 1048575 bytes
