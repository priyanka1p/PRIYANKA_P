// for client, port is dynamically assigned
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>   
#include <netinet/in.h>    
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000

int main() {
    int sockfd;
    struct sockaddr_in server_addr, peer_addr;
    socklen_t peer_len = sizeof(peer_addr);
    char peer_ip[INET_ADDRSTRLEN];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    // Get peer (server) address
    if (getpeername(sockfd, (struct sockaddr*)&peer_addr, &peer_len) == -1) {
        perror("getpeername");
        exit(1);
    }

    // Convert IP to readable format
    inet_ntop(AF_INET, &peer_addr.sin_addr, peer_ip, sizeof(peer_ip));

    printf("Connected to server at %s:%d\n", peer_ip, ntohs(peer_addr.sin_port));

    // Close socket
    close(sockfd);
    return 0;
}
