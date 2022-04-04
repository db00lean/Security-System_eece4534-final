// Server header file using 0MQ
#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef SERVER_H
#define SERVER_H

// Structure containing the information for server connection
struct server {
    void* context;
    zsock_t* responder;
    uint32_t port;
} server;

// Initializes a new server connection 
struct server* new_server(const char*);

// Recieves a new request from client connections
int receive_msg(zsock_t*);
#endif