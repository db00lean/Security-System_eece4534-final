// Client header file using 0MQ
#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef CLIENT_H
#define CLIENT_H

// Structure containing the information for 0mq client connection
struct client {
    void* context;
    zsock_t* requester;
    char* port;
} client;

// Initializes and returns a new client connection
struct client* new_client(const char*, const char*);

// Sends a new request to the previously initialized 0mq client
int send_msg(zsock_t*, char*, uint32_t);
#endif