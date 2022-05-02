// Server header file using 0MQ
#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "packet.h"
#ifndef SERVER_H
#define SERVER_H

#define MAX_CLIENTS 2
// Structure containing the information for server connection,
// This struct requires the clients to first register on the initaitlized port number, then a new registered client connection
// Will become available under the array of clients.
struct server {
    void* context;
    zsock_t* register_s;
    char reg_port[5];
    uint32_t num_clients;
    zsock_t* clients[MAX_CLIENTS];
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

// Registers a new client connection to this server, returns 0 if successful and -1 if there was an issue registering
int register_client(struct server*);

// Recieves a new request from client connections
received_message* receive_msg(zsock_t*);
#endif