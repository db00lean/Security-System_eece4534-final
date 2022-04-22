// Server header file using 0MQ
#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "packet.h"
#ifndef SERVER_H
#define SERVER_H

// Structure containing the information for server connection
struct server {
    void* context;
    zsock_t* responder;
    uint32_t port;
} server;

// Structure containing received message data and the associated packet header data
typedef struct received_message {
    int cam_id;
    PacketType type;
    int len;
    void* data;
} received_message;

// Initializes a new server connection 
struct server* new_server(const char*);

// Recieves a new request from client connections
received_message* receive_msg(zsock_t*);
#endif