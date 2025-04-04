#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 12345
#define BUFFER_SIZE 1024

// List of allowed commands
const char *allowed_cmds[] = {"ls", "whoami", "date", "uptime", NULL};

// Function to check if the command is allowed
int is_allowed_command(const char *cmd) {
    for (int i = 0; allowed_cmds[i] != NULL; i++) {
        if (strcmp(cmd, allowed_cmds[i]) == 0) {
            return 1;  // Command is allowed
        }
    }
    return 0;  // Command is not allowed
}

// Function to escape dangerous characters
void escape_command(const char *input, char *output, size_t output_size) {
    size_t j = 0;
    for (size_t i = 0; input[i] != '\0' && j < output_size - 2; i++) {  // -2 to reserve space for '\0'
        if (!isalnum(input[i]) && input[i] != ' ') {  // Escape non-alphanumeric characters
            if (j < output_size - 3) {  // -3 to ensure space for escape char, input char, and '\0'
                output[j++] = '\\';
            }
        }
        output[j++] = input[i];
    }
    output[j] = '\0';
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char command[BUFFER_SIZE];
    char escaped_command[BUFFER_SIZE * 2];
    char safe_command[BUFFER_SIZE * 2];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        printf("Client connected!\n");

        // Receive the command from client
        memset(command, 0, BUFFER_SIZE);
        recv(client_fd, command, BUFFER_SIZE - 1, 0);  // Ensure null termination
        command[BUFFER_SIZE - 1] = '\0';  // Explicitly null-terminate
        printf("Received command: %s\n", command);
        close(client_fd);

        // Extract the first word (actual command name)
        char cmd_name[BUFFER_SIZE];
        sscanf(command, "%s", cmd_name);// in the string command, extracting first word as a string and stroing it in variable cmd_name

        // Validate the command
        if (!is_allowed_command(cmd_name)) {
            printf("Error: Command '%s' is not allowed!\n", cmd_name);
            continue;
        }

        // Escape dangerous characters
        escape_command(command, escaped_command, sizeof(escaped_command));

        // Construct safe execution string
        //if my cmd is ls, escaped_command=ls, and safe_command ="ls >  /tmp/server.out 2>&1"
        //2>&1 2-stderr 1-stdout  It is shell syntax used for redirecting error output (stderr) to the same location as standard output (stdout).
        if (snprintf(safe_command, sizeof(safe_command), "%s > /tmp/server.out 2>&1", escaped_command) >= (int)sizeof(safe_command)) {
            printf("Error: Command is too long and may be truncated. Skipping execution.\n");
            continue;
        }

        // Execute the safe command
        system(safe_command);
        printf("Executed: %s\n", safe_command);
    }

    close(server_fd);
    return 0;
}
