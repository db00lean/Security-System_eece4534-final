#include <czmq.h>

#ifndef HELLO_H
#define HELLO_H

typedef struct server_hello {
    int cam_id;
    int port;
} server_hello;

typedef struct client_hello {
    int cam_id;
} client_hello;

// Structure containing received message data and the associated packet header data
typedef struct received_message {
    int cam_id;
    PacketType type;
    int len;
    void* data;
} received_message;


#endif