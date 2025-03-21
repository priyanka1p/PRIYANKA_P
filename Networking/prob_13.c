#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define HOSTNAME_LEN 256

int main() {
    char hostname[HOSTNAME_LEN];

    if (gethostname(hostname, HOSTNAME_LEN) == 0) {
        printf("Hostname: %s\n", hostname);

        struct addrinfo hints, *res;
        char ipstr[INET_ADDRSTRLEN];

        // Prepare hints
        hints.ai_family = AF_INET;  // Use IPv4
        hints.ai_socktype = 0;
        hints.ai_flags = AI_CANONNAME;
        hints.ai_protocol = 0;

        if (getaddrinfo(hostname, NULL, &hints, &res) == 0) {
            struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
            inet_ntop(AF_INET, &addr->sin_addr, ipstr, sizeof(ipstr));
            printf("IP Address: %s\n", ipstr);
            freeaddrinfo(res);
        } else {
            perror("getaddrinfo failed");
        }
    } else {
        perror("gethostname failed");
    }

    return 0;
}
