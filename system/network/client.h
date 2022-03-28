// Client header file using 0MQ
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef CLIENT_H
#define CLIENT_H

// Structure containing the information for 0mq client connection
struct client {
    void* context;
    void* requester;
    char port;
} client;

// Initializes and returns a new client connection
client new_client(const char* server_port, const char* server_address);

// Sends a new request to the previously initialized 0mq client
int send(void* requester, char* buff, uint32_t len);