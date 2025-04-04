#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9034
#define BUFFER_SIZE 256

int main(void) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    fd_set read_fds;
    struct timeval tv;
    int nbytes;

    // Step 1: Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Step 3: Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // Step 4: Start reading and sending messages
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds); // stdin (user input)
        FD_SET(sockfd, &read_fds); // server socket

        tv.tv_sec = 1; // Timeout of 1 second
        tv.tv_usec = 0;

        // Use select to allow non-blocking input and server response handling
        int ret = select(sockfd + 1, &read_fds, NULL, NULL, &tv);
        if (ret == -1) {
            perror("select error");
            break;
        }

        // If there is input from stdin
        if (FD_ISSET(0, &read_fds)) {
            // Get input from user
            printf("You: ");
            memset(buffer, 0, BUFFER_SIZE);
            if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
                printf("Error reading input.\n");
                continue;
            }

            // Remove newline character from buffer
            buffer[strcspn(buffer, "\n")] = '\0'; 

            if (strcmp(buffer, "exit") == 0) {
                printf("Exiting...\n");
                break;
            }

            // Send message to server
            if (send(sockfd, buffer, strlen(buffer), 0) == -1) {
                perror("Send failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            printf("Message sent to server: %s\n", buffer);
        }

        // If there's data from the server (other clients)
        if (FD_ISSET(sockfd, &read_fds)) {
            nbytes = recv(sockfd, buffer, BUFFER_SIZE, 0);
            if (nbytes == -1) {
                perror("Recv failed");
                close(sockfd);
                exit(EXIT_FAILURE);
            } else if (nbytes == 0) {
                printf("Server disconnected\n");
                break;
            } else {
                buffer[nbytes] = '\0';
                printf("Server: %s\n", buffer);
            }
        }
    }

    // Step 5: Close socket
    printf("Closing connection...\n");
    close(sockfd);
    return 0;
}
