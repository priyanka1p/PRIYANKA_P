#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>

#define PORT 8080
#define MAX_EVENTS 1024
#define BUFFER_SIZE 65536
#define UPLOAD_DIR "/var/www/uploads/"
#define WWW_DIR "/var/www/"
#include <ctype.h>

// Case-insensitive substring search (like strcasestr)
char* strcasestr(const char *haystack, const char *needle) {
    while (*haystack) {
        const char *h = haystack;//The string you want to search in.
        const char *n = needle;//substring u want to find
        
        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++;
            n++;
        }

        if (*n == '\0')  // Found the substring
            return (char*)haystack;

        haystack++;
    }
    return NULL;  // Substring not found
}


typedef struct {
    int fd;
    char buffer[BUFFER_SIZE];//store incoming data
    int received;
    int content_length;
    bool header_parsed;
} client_ctx;

client_ctx clients[MAX_EVENTS] = {0};

void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

client_ctx* get_client(int fd) {
    for (int i = 0; i < MAX_EVENTS; ++i)
        if (clients[i].fd == fd) return &clients[i];
    return NULL;
}

client_ctx* create_client(int fd) {
    for (int i = 0; i < MAX_EVENTS; ++i) {
        if (clients[i].fd == 0) {
            clients[i].fd = fd;
            clients[i].received = 0;
            clients[i].content_length = -1;
            clients[i].header_parsed = false;
            return &clients[i];
        }
    }
    return NULL;
}

void remove_client(int fd) {
    for (int i = 0; i < MAX_EVENTS; ++i)
        if (clients[i].fd == fd) clients[i].fd = 0;
}

const char* get_mime_type(const char* path) {
    const char* ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".jpg") == 0) return "image/jpeg";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".pdf") == 0) return "application/pdf";
    return "application/octet-stream";
}

void send_404(int client_fd) {
    const char* msg = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
    send(client_fd, msg, strlen(msg), 0);
}

void handle_file_upload(const char* buffer) {
	
    const char* filename_pos = strstr(buffer, "filename=\"");
    if (!filename_pos) return;
    filename_pos += 10;
    const char* filename_end = strchr(filename_pos, '"');
    if (!filename_end) return;

    char filename[256] = {0};
    strncpy(filename, filename_pos, filename_end - filename_pos);

    const char* content_start = strstr(filename_end, "\r\n\r\n");
    if (!content_start) return;
    content_start += 4;

    const char* boundary = strstr(buffer, "\r\n--");
    const char* content_end = boundary ? strstr(content_start, boundary) : NULL;
    int content_len = content_end ? content_end - content_start : strlen(content_start);

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s%s", UPLOAD_DIR, filename);
    FILE* fp = fopen(filepath, "wb");
    if (fp) {
        fwrite(content_start, 1, content_len, fp);
        fclose(fp);
    }
}
void post_data(const char* buffer,int client_fd,int body_len){
	const char* content_length_header = strstr(buffer, "\r\n\r\n");
        if (!content_length_header) {
            printf("Missing Content-Length\n");
            close(client_fd);
            return;
        }
        const char* body_start =  content_length_header+ 4;
	char body[256] = {0};
    	strncpy(body, body_start, body_len);
        

	FILE* f = fopen(UPLOAD_DIR "/form_data.txt", "a");
            if (f) {
                fprintf(f, "%s\n", body);
                fclose(f);
            }

            char response[1024];
            snprintf(response, sizeof(response),
                     "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\nReceived POST data:\n%s\n", body);
            send(client_fd, response, strlen(response), 0);
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
        }
        
       

void serve_file(int client_fd, const char* path) {
    char fullpath[512];
    printf("path: %s\n",path);
    snprintf(fullpath, sizeof(fullpath), "%s%s", WWW_DIR, path[1] ? path + 1 : "index.html");

    FILE* fp = fopen(fullpath, "rb");
    if (!fp) return send_404(client_fd);

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    char* filedata = malloc(filesize);
    fread(filedata, 1, filesize, fp);
    fclose(fp);

    const char* mime = get_mime_type(fullpath);
    char header[512];
    snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: %s\r\n\r\n",
        filesize, mime);
    send(client_fd, header, strlen(header), 0);
    send(client_fd, filedata, filesize, 0);
    free(filedata);
}

void handle_request(int client_fd,int body_len) {
    client_ctx* ctx = get_client(client_fd);
    if (!ctx) return;

    char* buffer = ctx->buffer;
    printf("buffer: %s\n",buffer);
    printf("****#######################**\n");
    if (strncmp(buffer, "GET ", 4) == 0) {
        char path[1024];
        sscanf(buffer, "GET %s", path);
        serve_file(client_fd, path);
    } else if (strncmp(buffer, "POST ", 5) == 0) {
        if (strstr(buffer, "multipart/form-data")) {
            handle_file_upload(buffer);
            const char* ok = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
            send(client_fd, ok, strlen(ok), 0);
        } 
       else if (strstr(buffer, "application/x-www-form-urlencoded")) {
            post_data(buffer,client_fd,body_len);
        }
    } else {
        send_404(client_fd);
    }
}

int main() {
    mkdir(UPLOAD_DIR, 0755);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    set_nonblocking(server_fd);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 128);

    int epoll_fd = epoll_create1(0);//epoll instance
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;//file descriptor is ready for reading.
    ev.data.fd = server_fd;//registers 
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);//adds in the interest list

    printf("HTTP Server running on port %d (epoll level-triggered)\n", PORT);

    while (1) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);//untill event happens
        //n is the number of ready file descriptors returned by epoll_wait().
        for (int i = 0; i < n; ++i) {
            int fd = events[i].data.fd;//tore the file descriptor in data.fd.

            if (fd == server_fd) {//accepting new connections
                struct sockaddr_in cli;
                socklen_t len = sizeof(cli);
                int client_fd = accept(server_fd, (struct sockaddr*)&cli, &len);
                if (client_fd >= 0) {
                    set_nonblocking(client_fd);
                    ev.events = EPOLLIN;
                    ev.data.fd = client_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                    create_client(client_fd);
                }
            } else {
                client_ctx* ctx = get_client(fd);
                if (!ctx) continue;

                int r = recv(fd, ctx->buffer + ctx->received, BUFFER_SIZE - ctx->received - 1, 0);
//ctx->buffer + ctx->receivedpoints to the next free position in the buffer where new data should be written.
                if (r <= 0) {
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    remove_client(fd);
                    continue;
                }

                ctx->received += r;
                ctx->buffer[ctx->received] = '\0';
//to check if complete header is received or not
                if (!ctx->header_parsed) {
                    char* header_end = strstr(ctx->buffer, "\r\n\r\n");
                    if (header_end) {
                        ctx->header_parsed = true;
                        char* cl = strcasestr(ctx->buffer, "Content-Length:");
                        if (cl) {
                            sscanf(cl, "Content-Length: %d", &ctx->content_length);
                        } else {
                            ctx->content_length = 0;
                        }
                    }
                }

                if (ctx->header_parsed) {
                    char* body = strstr(ctx->buffer, "\r\n\r\n");
                    if (!body) continue;
                    int body_len = ctx->received - (body + 4 - ctx->buffer);
                    if (body_len >= ctx->content_length) {
                        handle_request(fd,body_len);
                        close(fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                        remove_client(fd);
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
