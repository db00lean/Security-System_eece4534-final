// Server header file using 0MQ
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef SERVER_H
#define SERVER_H

// Structure containing the information for server connection
struct 0mq_server {
    void* context;
    void* responder;
    uint32_t port;
}

// Initializes a new 0mq_server connection 
0mq_server new_server(uint32_t port);

// Recieves a new request from client connections
int receive(0mq_server conn);