/*Rewrite the program cat from Chapter 7 using read, write, open, and close
instead of their standard library equivalents. Perform experiments to determine the relative
speeds of the two versions.*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096 

void filecopy(int fd_src, int fd_dest) {
    char buffer[BUFFER_SIZE];
    ssize_t n;
    while ((n = read(fd_src, buffer, BUFFER_SIZE)) > 0) {
        if (write(fd_dest, buffer, n) != n) {
            perror("write error");
            exit(1);
        }
    }
    
    if (n < 0) {
        perror("read error");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int fd;
    
    if (argc == 1) {
        filecopy(STDIN_FILENO, STDOUT_FILENO);
    } else {
        for (int i = 1; i < argc; i++) {
            fd = open(argv[i], O_RDONLY);
            if (fd < 0) {
                perror("open error");
                exit(1);
            }
            filecopy(fd, STDOUT_FILENO);
            close(fd);
        }
    }
    
    return 0;
}
/*my inspiration is rishabh pant.
my inspiration is rishabh pant.
i will open a shop in future
i will open a shop in future
*/
