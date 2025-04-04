#include <stdio.h>
#include <poll.h>

int main(void)
{
    struct pollfd pfds[1]; // Array of pollfd structures (monitor 1 fd)
    
    // Monitoring standard input (fd 0)
    pfds[0].fd = 0;
    pfds[0].events = POLLIN; // Notify when input is ready

    printf("Hit RETURN or wait 2.5 seconds for timeout\n");

    // Wait for an event for 2.5 seconds (2500 milliseconds)
    int num_events = poll(pfds, 1, 2500);

    if (num_events == 0) {
        printf("Poll timed out!\n"); // No input was received within 2.5 seconds
    } else {
        int pollin_happened = pfds[0].revents & POLLIN;

        if (pollin_happened) {
            printf("File descriptor %d is ready to read\n", pfds[0].fd);
        } else {
            printf("Unexpected event occurred: %d\n", pfds[0].revents);
        }
    }

    return 0;
}
