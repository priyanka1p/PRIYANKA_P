#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 5000

int main() {
    int sockfd, new_fd;
    struct sockaddr_in server_addr, client_addr, peer_addr;
    socklen_t client_len = sizeof(client_addr), peer_len = sizeof(peer_addr);
    char client_ip[INET_ADDRSTRLEN], peer_ip[INET_ADDRSTRLEN];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for connections
    listen(sockfd, 5);
    printf("Waiting for connections...\n");

    // Accept client connection
    new_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (new_fd == -1) {
        perror("accept");
        exit(1);
    }

    // Convert and print client address
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Get peer (client) address
    if (getpeername(new_fd, (struct sockaddr*)&peer_addr, &peer_len) == -1) {
        perror("getpeername");
        exit(1);
    }

    // Convert and print peer address
    inet_ntop(AF_INET, &peer_addr.sin_addr, peer_ip, sizeof(peer_ip));
    printf("Peer (client) info: %s:%d\n", peer_ip, ntohs(peer_addr.sin_port));

    // Close sockets
    close(new_fd);
    close(sockfd);
    return 0;
}
