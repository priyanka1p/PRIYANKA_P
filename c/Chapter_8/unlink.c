//removes the file from its system/folder.
#include <stdio.h>
#include <unistd.h>

int remove(const char *filename) {
    return unlink(filename);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (remove(argv[1]) == 0)
        printf("File '%s' deleted successfully.\n", argv[1]);
    else
        perror("Error deleting file");

    return 0;
}
