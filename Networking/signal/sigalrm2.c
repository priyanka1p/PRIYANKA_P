//signal.c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void print_time(const char *msg) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[%02d:%02d:%02d] %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
}

void timeout(int sig) {
    if (sig == SIGALRM) {
        print_time("Time out!");
        alarm(5);  // Schedule the next SIGALRM in 5 seconds
    }
}

int main() {
    signal(SIGALRM, timeout);  // Set signal handler
    alarm(5);  // First alarm after 5 seconds

    for (int i = 0; i < 3; i++) {
        print_time("wait...");
        int remaining = 15;
        while (remaining > 0) {
            remaining = sleep(remaining);  // Interrupted sleep will return remaining time
        } // Sleep for 15 seconds (3 timeouts will happen in this period)
        print_time("hhhhhhhhhhhhhhh");
    }

    return 0;
}
