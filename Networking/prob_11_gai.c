//domain name to IP address using getaddrinfo
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    struct addrinfo hints, *results, *p;
    char hostname[100], ipstr[INET6_ADDRSTRLEN];
    int rv;

    printf("Enter a Domain Name: \n");
    scanf("%s", hostname);
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, NULL, &hints, &results)\n", gai_st) != 0) {
        fprintf(stderr, "getaddrinfo: %srerror(rv));
        return 1;
    }
    for (p = results; p != NULL; p = p->ai_next) {
        void *addr;
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("IP Address: %s\n", ipstr);
    }

    freeaddrinfo(results);
    return 0;
}
