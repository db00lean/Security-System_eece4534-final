// Client header file using 0MQ
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef CLIENT_H
#define CLIENT_H

// Structure containing the information for 0mq client connection
struct 0mq_client {
    void* context;
    void* requester;
    uint32_t port;
}

// Initializes and returns a new client connection
0mq_client new_client(uint32_t port);

// Sends a new request to the previously initialized 0mq client
int send(0mq_client conn);