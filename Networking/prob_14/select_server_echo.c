#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define BACKLOG 5
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_fd, max_fd, activity;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    fd_set master_set, read_fds;
    char buffer[BUFFER_SIZE];

    // Step 1: Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Step 3: Listen
    if (listen(server_fd, BACKLOG) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Step 4: Initialize `fd_set`
    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    max_fd = server_fd;

    while (1) {
        read_fds = master_set;  // Copy master set (select modifies it)
        
        // Step 5: Use select()
        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        
        if (activity == -1) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Step 6: Check for new connections
        if (FD_ISSET(server_fd, &read_fds)) {
            addr_size = sizeof(client_addr);
            new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
            if (new_fd == -1) {
                perror("Accept failed");
                continue;
            }

            printf("New connection: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            FD_SET(new_fd, &master_set);  // Add new client socket
            if (new_fd > max_fd) {
                max_fd = new_fd;  // Update max file descriptor
            }
        }

        // Step 7: Check for data from clients
        for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i != server_fd) {  // Ignore listening socket
                    memset(buffer, 0, BUFFER_SIZE);
                    int bytes_received = recv(i, buffer, BUFFER_SIZE, 0);
                    
                    if (bytes_received <= 0) {
                        if (bytes_received == 0) {
                            printf("Client disconnected: FD %d\n", i);
                        } else {
                            perror("Recv error");
                        }
                        close(i);
                        FD_CLR(i, &master_set);
                    } else {
                        printf("Received: %s\n", buffer);
                        send(i, buffer, bytes_received, 0);  // Echo back
                    }
                }
            }
        }
    }
    
    close(server_fd);
    return 0;
}
