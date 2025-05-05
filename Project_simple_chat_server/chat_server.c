#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <openssl/sha.h>
#include <cjson/cJSON.h>
#include <ctype.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 8192

struct client_info {
    int socket;
    char username[100];
};

struct client_info clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Base64 encoding table
static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *base64_encode(const unsigned char *in, size_t len) {
//Base64 encoding is used to represent binary data (like hashes or raw bytes) using only printable ASCII characters
    char *out = malloc((len + 2) / 3 * 4 + 1);
    int val = 0, valb = -6;
    size_t i, j = 0;
    for (i = 0; i < len; i++) {
        val = (val << 8) + in[i];
        valb += 8;
        while (valb >= 0) {
            out[j++] = b64_table[(val >> valb) & 0x3F];
            valb -= 6;
        }
    }
    if (valb > -6) out[j++] = b64_table[((val << 8) >> (valb + 8)) & 0x3F];
    while (j % 4) out[j++] = '=';
    out[j] = '\0';
    return out;
}

int websocket_handshake(int client_socket, char *buffer) {
    char *key = strstr(buffer, "Sec-WebSocket-Key: ");
    if (!key) return -1;
    key += strlen("Sec-WebSocket-Key: ");
    char *end = strstr(key, "\r\n");
    if (!end) return -1;
    char client_key[256] = {0};
    strncpy(client_key, key, end - key);
//Globally Unique Identifier (GUID) specified by the WebSocket protocol for use during the handshake process.
/*Concatenate that key with the GUID:
Take the SHA-1 hash of that string.
Base64-encode the result.
Return it in the HTTP response header
*/
    const char *GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    char concatenated[256];
    snprintf(concatenated, sizeof(concatenated), "%s%s", client_key, GUID);

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)concatenated, strlen(concatenated), hash);
    char *accept_key = base64_encode(hash, SHA_DIGEST_LENGTH);

    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 101 Switching Protocols\r\n"
             "Upgrade: websocket\r\n"
             "Connection: Upgrade\r\n"
             "Sec-WebSocket-Accept: %s\r\n\r\n",
             accept_key);

    send(client_socket, response, strlen(response), 0);
    free(accept_key);
    return 0;
}

int recv_ws_frame(int client_socket, char *data, int len) {
    unsigned char buffer[BUFFER_SIZE];
    int received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (received <= 0) return -1;

    int payload_len = buffer[1] & 0x7F;
    int mask_offset = 2;
    if (payload_len == 126) mask_offset = 4;
    else if (payload_len == 127) mask_offset = 10;

    unsigned char mask[4];
    memcpy(mask, buffer + mask_offset, 4);

    int data_offset = mask_offset + 4;
    int data_len = received - data_offset;

    for (int i = 0; i < data_len && i < len - 1; i++) {
        data[i] = buffer[data_offset + i] ^ mask[i % 4];
    }
    data[data_len] = '\0';
    return data_len;
}

int send_ws_frame(int client_socket, const char *message) {
    size_t len = strlen(message);
    unsigned char frame[BUFFER_SIZE] = {0};
    frame[0] = 0x81;
    if (len <= 125) {
        frame[1] = len;
        memcpy(frame + 2, message, len);
        send(client_socket, frame, len + 2, 0);
    } else if (len <= 65535) {
        frame[1] = 126;
        frame[2] = (len >> 8) & 0xFF;
        frame[3] = len & 0xFF;
        memcpy(frame + 4, message, len);
        send(client_socket, frame, len + 4, 0);
    } else {
        frame[1] = 127;
        // Not supporting lengths > 2^16 for simplicity
        return -1;
    }
    return 0;
}

void send_to_user(const char *username, const char *msg) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != 0 && strcmp(clients[i].username, username) == 0) {
            send_ws_frame(clients[i].socket, msg);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast(const char *msg) {
/* msg includes Chat messages, User join/leave notifications, Updated user lists (in JSON)*/
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != 0) {
            send_ws_frame(clients[i].socket, msg);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_user_list() {
//JSON is used to structure and exchange data between clients and the server using the cJSON librar
    cJSON *root = cJSON_CreateObject();   //{} empty json object
    cJSON_AddStringToObject(root, "type", "userlist");  //adds key value pair {"type": "userlist"}
    cJSON *users = cJSON_AddArrayToObject(root, "users"); //returns a pointer to this newly created array (users) so that you can later add usernames to it.



    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket != 0) {
            cJSON_AddItemToArray(users, cJSON_CreateString(clients[i].username));//adding usernames to the users array
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    char *msg = cJSON_PrintUnformatted(root);//json object to string
    broadcast(msg);
    cJSON_Delete(root);//Free JSON object memory
    free(msg);
}

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
//HTTP upgrade request
/*
GET /chat HTTP/1.1
Host: localhost:8080
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Sec-WebSocket-Version: 13*/

    recv(client_socket, buffer, sizeof(buffer), 0); //containing the HTTP upgrade request (headers sent by the browser)
    printf("buffr:  %s\n",buffer);
    websocket_handshake(client_socket, buffer);
//It reads the HTTP Upgrade request sent by the browser, extracts the Sec-WebSocket-Key header, 
//computes the correct WebSocket accept key, and sends back an appropriate HTTP 101 Switching Protocols response.
/*
| Step                     | Purpose                                                       |
| ------------------------ | ------------------------------------------------------------- |
| Read `Sec-WebSocket-Key` | Identify the client's WebSocket request                       |
| Append WebSocket GUID    | Required by RFC 6455 standard                                 |
| SHA1 + Base64 encode     | Generate response token to prove server understands WebSocket |
| Send HTTP 101 response   | Upgrade the protocol from HTTP to WebSocket                   |
*/
    memset(buffer, 0, sizeof(buffer));
    
    
    recv_ws_frame(client_socket, buffer, sizeof(buffer));
    // If the browser sends "hello", recv_ws_frame decodes the raw WebSocket frame and stores "hello" in buffer.
    char username[100];
    strncpy(username, buffer, sizeof(username));

    pthread_mutex_lock(&clients_mutex);
    //adding clients in array, no two clients can access the array simultaneiusly
    int index = -1;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].socket == 0) {
            clients[i].socket = client_socket;
            strcpy(clients[i].username, username);
            //storing the username and the socket descriptor
            index = i;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    broadcast_user_list();

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        if (recv_ws_frame(client_socket, buffer, sizeof(buffer)) <= 0)
            break;

        cJSON *root = cJSON_Parse(buffer);
        //Parses the received JSON string (from WebSocket message) into a cJSON object.
        //{"type": "message","from": "Alice","to": "Bob","message": "Hi Bob!"}

        if (!root) continue;

        const cJSON *type = cJSON_GetObjectItem(root, "type");
        if (type && strcmp(type->valuestring, "message") == 0) {
            const char *from = cJSON_GetObjectItem(root, "from")->valuestring;
            const char *to = cJSON_GetObjectItem(root, "to")->valuestring;
            const char *msg = cJSON_GetObjectItem(root, "message")->valuestring;

            printf("[MSG] From: %s, To: %s, Message: %s\n", from, to, msg);

            cJSON *out = cJSON_CreateObject();
            cJSON_AddStringToObject(out, "type", "message");
            cJSON_AddStringToObject(out, "from", from);
            cJSON_AddStringToObject(out, "to", to);
            cJSON_AddStringToObject(out, "message", msg);

            char *out_msg = cJSON_PrintUnformatted(out);
            if (strcmp(to, "all") == 0) {
                broadcast(out_msg);
            } else {
                send_to_user(to, out_msg);
            }
            cJSON_Delete(out);
            free(out_msg);
        }
        cJSON_Delete(root);
    }

    close(client_socket);
    pthread_mutex_lock(&clients_mutex);
    clients[index].socket = 0;
    clients[index].username[0] = 0;
    pthread_mutex_unlock(&clients_mutex);
    broadcast_user_list();
    return NULL;
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Chat server started on port 8080...\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        pthread_t thread;
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_socket;
        pthread_create(&thread, NULL, handle_client, client_socket_ptr);
        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}
