#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PORT 8080
#define BUFFER_SIZE 8192
#define WEB_ROOT "/var/www"
#define UPLOAD_DIR "/var/www/uploads"

const char* get_content_type(const char* path) {
    if (strstr(path, ".html")) return "text/html";
    if (strstr(path, ".jpg")) return "image/jpeg";
    if (strstr(path, ".png")) return "image/png";
    if (strstr(path, ".gif")) return "image/gif";
    if (strstr(path, ".css")) return "text/css";
    if (strstr(path, ".js")) return "application/javascript";
    if (strstr(path, ".pdf")) return "application/pdf";
    if (strstr(path, ".mp4")) return "video/mp4";
    if (strstr(path, ".mp3")) return "audio/mpeg";
    return "application/octet-stream";
}

void save_uploaded_file(const char* boundary, const char* body, const char* end) {
    const char* file_start = strstr(body, "\r\n\r\n");
    if (!file_start) return;
    file_start += 4;

    const char* file_end = strstr(file_start, boundary);
    if (!file_end) return;

    const char* filename_marker = strstr(body, "filename=\"");
    if (!filename_marker) return;
    filename_marker += 10;
    const char* filename_end = strchr(filename_marker, '"');
    if (!filename_end) return;

    char filename[256];
    snprintf(filename, filename_end - filename_marker + 1, "%s", filename_marker);

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%s", UPLOAD_DIR, filename);

    FILE* fp = fopen(filepath, "wb");
    if (fp) {
        fwrite(file_start, 1, file_end - file_start - 2, fp); // -2 to remove \r\n
        fclose(fp);
        printf("Saved file: %s\n", filepath);
    } else {
        perror("File write failed");
    }
}

void handle_client(int client_fd, struct sockaddr_in client_addr) {
    char buffer[BUFFER_SIZE];
    int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes <= 0) {
        close(client_fd);
        return;
    }

    buffer[bytes] = '\0';

    char method[16], path_raw[2048], protocol[16];
    sscanf(buffer, "%15s %2047s %15s", method, path_raw, protocol);

    if (strcmp(method, "GET") == 0) {
        char path[8192];
        if (strcmp(path_raw, "/") == 0) {
            snprintf(path, sizeof(path), "%s/index.html", WEB_ROOT);
        } else {
            snprintf(path, sizeof(path), "%s%s", WEB_ROOT, path_raw);
        }

        FILE* file = fopen(path, "rb");
        if (!file) {
            const char* not_found = "HTTP/1.0 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile not found.\n";
            send(client_fd, not_found, strlen(not_found), 0);
        } else {
            fseek(file, 0, SEEK_END);
            long filesize = ftell(file);
            rewind(file);

            const char* content_type = get_content_type(path);
            char header[512];
            snprintf(header, sizeof(header),
                     "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n",
                     content_type, filesize);
            send(client_fd, header, strlen(header), 0);

            size_t n;
            while ((n = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                send(client_fd, buffer, n, 0);
            }

            fclose(file);
        }

    } else if (strcmp(method, "POST") == 0) {
        const char* content_type_header = strstr(buffer, "Content-Type: ");
        if (!content_type_header) {
            close(client_fd);
            return;
        }

        if (strstr(content_type_header, "multipart/form-data")) {
            const char* boundary_marker = strstr(content_type_header, "boundary=");
            if (!boundary_marker) {
                close(client_fd);
                return;
            }

            char boundary[100];
            sscanf(boundary_marker, "boundary=%99s", boundary);
            char real_boundary[120] = "--";
            strcat(real_boundary, boundary);

            char* body_start = strstr(buffer, "\r\n\r\n");
            if (!body_start) {
                close(client_fd);
                return;
            }

            body_start += 4;
            save_uploaded_file(real_boundary, body_start, buffer + bytes);

            const char* ok_response = "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\nFile uploaded successfully.\n";
            send(client_fd, ok_response, strlen(ok_response), 0);

        } else if (strstr(content_type_header, "application/x-www-form-urlencoded")) {
            char* body = strstr(buffer, "\r\n\r\n");
            if (body) {
                body += 4;
                printf("Form data received: %s\n", body);

                char response[1024];
                snprintf(response, sizeof(response),
                         "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\nReceived POST data:\n%s\n", body);
                send(client_fd, response, strlen(response), 0);
            }
        } else {
            const char* unsupported = "HTTP/1.0 415 Unsupported Media Type\r\nContent-Type: text/plain\r\n\r\nUnsupported Content-Type.\n";
            send(client_fd, unsupported, strlen(unsupported), 0);
        }

    } else {
        const char* not_supported = "HTTP/1.0 501 Not Implemented\r\nContent-Type: text/plain\r\n\r\nOnly GET and POST supported.\n";
        send(client_fd, not_supported, strlen(not_supported), 0);
    }

    close(client_fd);
}

int main() {
    mkdir(UPLOAD_DIR, 0755);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("HTTP Server running at http://localhost:%d\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd >= 0) {
            printf("Client connected: %s:%d\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            handle_client(client_fd, client_addr);
        }
    }

    close(server_fd);
    return 0;
}
