// Server header file using 0MQ
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifndef SERVER_H
#define SERVER_H

// Structure containing the information for server connection
struct server {
    void* context;
    void* responder;
    uint32_t port;
} server;

// Initializes a new server connection 
server new_server(const char* port);

// Recieves a new request from client connections
int receive(server conn);