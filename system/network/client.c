#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <zmq.h>

// Initializes and returns a new client connection
struct client new_client(const char* server_port, const char* server_address)
{
    int err;
    struct client c;
    char* bind_addr = "tcp://";
    char* semi = ":";
    // Initialize the context and the requester socket
    void* context = zmq_ctx_new();
    void* requester = zmq_socket(context, ZMQ_REQ);
    // Bind requester to socket using the given server information
    bind_addr = strcat(bind_addr, server_port);
    bind_addr = strcat(bind_addr, semi);
    bind_addr = strcat(bind_addr, server_address);
    err = zmq_bind(requester, bind_addr);
    assert (err == 0);
    c.context = context;
    c.requester = requester;
    return c;
}

// Sends a new request to the previously initialized 0mq client
int send(void* requester, char* buff, uint32_t len)
{
    char recv[10];
    printf("Sending %s to server\n", buff);
    zmq_send(requester, buff, len, 0);
    zmq_recv(requester, recv, 10, 0);
    printf("Received %s from server\n", recv);
}