#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server socket to non-blocking
    fcntl(server_fd, F_SETFL, O_NONBLOCK);

    // Bind server to address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Start listening
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d (non-blocking mode)...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (client_fd > 0) {
            printf("New client connected!\n");

            // Set client socket to non-blocking
            fcntl(client_fd, F_SETFL, O_NONBLOCK);

            while (1) {
                int bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
                
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("Received: %s\n", buffer);

                    // Echo back the message
                    send(client_fd, buffer, bytes_read, 0);
                    printf("Echoed back: %s\n", buffer);
                } 
                else if (bytes_read == 0) {
                    printf("Client disconnected.\n");
                    close(client_fd);
                    break;
                }
                else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // No data available yet, continue checking
                    usleep(100000);
                    continue;
                } 
                else {
                    perror("Recv failed");
                    close(client_fd);
                    break;
                }
            }
        }
        else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            perror("Accept failed");
        }

        usleep(100000);  // Prevent CPU overuse
    }

    close(server_fd);
    return 0;
}
