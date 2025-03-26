#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    fd_set read_fds;
    int activity;

    // Step 1: Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // Step 3: Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        // Step 4: Use select() to check for input from both stdin and server
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);  // Monitor standard input
        FD_SET(client_fd, &read_fds);     // Monitor socket

        activity = select(client_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity == -1) {
            perror("Select error");
            close(client_fd);
            exit(EXIT_FAILURE);
        }

        // Step 5: Check if user input is available
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline

            if (strcmp(buffer, "exit") == 0) {
                printf("Exiting...\n");
                break;
            }

            // Send message to the server
            if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
                perror("Send failed");
            }
        }

        // Step 6: Check if server sent a response
        if (FD_ISSET(client_fd, &read_fds)) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                if (bytes_received == 0) {
                    printf("Server disconnected\n");
                } else {
                    perror("Recv error");
                }
                break;
            }

            printf("Server: %s\n", buffer);
        }
    }

    // Step 7: Close socket
    close(client_fd);
    return 0;
}
