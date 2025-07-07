// chat_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>
#include <cjson/cJSON.h>

#define MAX_CLIENTS 20
#define MAX_USERNAME_LEN 64
#define MAX_MSG_LEN 512

struct client_info {
    struct lws *wsi;
    char username[MAX_USERNAME_LEN];
};

static struct client_info clients[MAX_CLIENTS];

static int find_client_index(struct lws *wsi) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].wsi == wsi) return i;
    }
    return -1;
}

static int find_client_by_name(const char *username) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].wsi != NULL && strcmp(clients[i].username, username) == 0)
            return i;
    }
    return -1;
}

static int add_client(struct lws *wsi, const char *username) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].wsi == NULL) {
            clients[i].wsi = wsi;
            strncpy(clients[i].username, username, MAX_USERNAME_LEN-1);
            clients[i].username[MAX_USERNAME_LEN-1] = 0;
            return i;
        }
    }
    return -1; // full
}

static void remove_client(struct lws *wsi) {
    int idx = find_client_index(wsi);
    if (idx != -1) {
        clients[idx].wsi = NULL;
        clients[idx].username[0] = 0;
    }
}

static int is_username_taken(const char *username) {
    return find_client_by_name(username) != -1;
}

// Broadcast current user list to all clients
static void broadcast_userlist(struct lws_context *context) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "type", "userlist");
    cJSON *users = cJSON_CreateArray();

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].wsi != NULL) {
            cJSON_AddItemToArray(users, cJSON_CreateString(clients[i].username));
        }
    }
    cJSON_AddItemToObject(root, "users", users);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    unsigned char buf[LWS_PRE + MAX_MSG_LEN];
    size_t len = strlen(json_str);
    if (len > MAX_MSG_LEN) len = MAX_MSG_LEN;

    memcpy(&buf[LWS_PRE], json_str, len);
    free(json_str);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].wsi != NULL) {
            lws_write(clients[i].wsi, &buf[LWS_PRE], len, LWS_WRITE_TEXT);
        }
    }
}

// Send JSON message to one client
static void send_json_to_client(struct lws *wsi, cJSON *json) {
    char *json_str = cJSON_PrintUnformatted(json);
    if (!json_str) return;

    unsigned char buf[LWS_PRE + MAX_MSG_LEN];
    size_t len = strlen(json_str);
    if (len > MAX_MSG_LEN) len = MAX_MSG_LEN;

    memcpy(&buf[LWS_PRE], json_str, len);
    free(json_str);

    lws_write(wsi, &buf[LWS_PRE], len, LWS_WRITE_TEXT);
}

static void broadcast_message(const char *from, const char *message) {
    cJSON *msg_json = cJSON_CreateObject();
    cJSON_AddStringToObject(msg_json, "type", "message");
    cJSON_AddStringToObject(msg_json, "from", from);
    cJSON_AddStringToObject(msg_json, "to", "all");
    cJSON_AddStringToObject(msg_json, "message", message);

    char *json_str = cJSON_PrintUnformatted(msg_json);
    cJSON_Delete(msg_json);

    unsigned char buf[LWS_PRE + MAX_MSG_LEN];
    size_t len = strlen(json_str);
    if (len > MAX_MSG_LEN) len = MAX_MSG_LEN;

    memcpy(&buf[LWS_PRE], json_str, len);
    free(json_str);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].wsi != NULL) {
            lws_write(clients[i].wsi, &buf[LWS_PRE], len, LWS_WRITE_TEXT);
        }
    }
}

static int callback_chat(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len) {
    switch (reason) {
    case LWS_CALLBACK_ESTABLISHED:
        printf("Client connected\n");
        break;

    case LWS_CALLBACK_RECEIVE: {
        char *msg = (char *)in;
        int idx = find_client_index(wsi);

        if (idx == -1) {
            // First message = username registration
            if (len >= MAX_USERNAME_LEN) len = MAX_USERNAME_LEN - 1;
            char username[MAX_USERNAME_LEN];
            memcpy(username, msg, len);
            username[len] = 0;

            if (is_username_taken(username)) {
                // Send error and disconnect client
                cJSON *err = cJSON_CreateObject();
                cJSON_AddStringToObject(err, "type", "error");
                cJSON_AddStringToObject(err, "message", "Username already taken");
                send_json_to_client(wsi, err);
                cJSON_Delete(err);

                return -1; // disconnect
            }

            add_client(wsi, username);
            broadcast_userlist(lws_get_context(wsi));
            printf("User registered: %s\n", username);
            break;
        }

        // Parse received JSON
        cJSON *root = cJSON_ParseWithLength(msg, len);
        if (!root) break;

        cJSON *type = cJSON_GetObjectItem(root, "type");
        if (!type || type->type != cJSON_String) {
            cJSON_Delete(root);
            break;
        }

        if (strcmp(type->valuestring, "message") == 0) {
    cJSON *from = cJSON_GetObjectItem(root, "from");
    cJSON *to = cJSON_GetObjectItem(root, "to");
    cJSON *message = cJSON_GetObjectItem(root, "message");

    if (from && to && message &&
        from->type == cJSON_String &&
        to->type == cJSON_String &&
        message->type == cJSON_String) {

        int target_idx = find_client_by_name(to->valuestring);

        // Create the message JSON object once
        cJSON *msg_json = cJSON_CreateObject();
        cJSON_AddStringToObject(msg_json, "type", "message");
        cJSON_AddStringToObject(msg_json, "from", from->valuestring);
        cJSON_AddStringToObject(msg_json, "to", to->valuestring);
        cJSON_AddStringToObject(msg_json, "message", message->valuestring);

        if (strcmp(to->valuestring, "all") == 0) {
            // Broadcast to all users
            char *json_str = cJSON_PrintUnformatted(msg_json);
            unsigned char buf[LWS_PRE + MAX_MSG_LEN];
            size_t len = strlen(json_str);
            if (len > MAX_MSG_LEN) len = MAX_MSG_LEN;
            memcpy(&buf[LWS_PRE], json_str, len);
            free(json_str);

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].wsi != NULL) {
                    lws_write(clients[i].wsi, &buf[LWS_PRE], len, LWS_WRITE_TEXT);
                }
            }
        } else if (target_idx != -1) {
            // Private message — send to both sender and recipient
            send_json_to_client(clients[target_idx].wsi, msg_json);  // to recipient
            send_json_to_client(wsi, msg_json);                      // back to sender
        } else {
            // Recipient offline — broadcast a note
            char broadcast_msg[MAX_MSG_LEN];
            snprintf(broadcast_msg, MAX_MSG_LEN,
                     "(%s tried to send private msg to offline user %s): %s",
                     from->valuestring, to->valuestring, message->valuestring);
            broadcast_message(from->valuestring, broadcast_msg);
        }

        cJSON_Delete(msg_json);
    }
}

        cJSON_Delete(root);
        break;
    }

    case LWS_CALLBACK_CLOSED:
        printf("Client disconnected\n");
        remove_client(wsi);
        broadcast_userlist(lws_get_context(wsi));
        break;

    default:
        break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    {
        .name = "chat-protocol",
        .callback = callback_chat,
        .per_session_data_size = 0,
        .rx_buffer_size = 0,
    },
    { NULL, NULL, 0, 0 }
};

int main(void) {
    struct lws_context_creation_info info;
    struct lws_context *context;

    memset(clients, 0, sizeof(clients));
    memset(&info, 0, sizeof(info));

    info.port = 9000;
    info.protocols = protocols;
    info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "lws init failed\n");
        return 1;
    }

    printf("Server started on port %d\n", info.port);

    while (1) {
        lws_service(context, 1000);
    }

    lws_context_destroy(context);

    return 0;
}
