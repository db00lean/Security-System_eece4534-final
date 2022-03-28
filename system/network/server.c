#include <zmq.h>
#include "server.h"
#include <unistd.h>
#include <string.h>
#include <assert.h>

// Initializes a new server connection 
struct server new_server(const char* port)
{
    int err;
    struct server s;
    char* bind_addr = "tcp://*:";
    // Initialize the context and the requester socket
    void* context = zmq_ctx_new();
    void* responder = zmq_socket(context, ZMQ_REP);
    // Bind requester to socket using the given server information
    bind_addr = strcat(bind_addr, port);
    err = zmq_bind(responder, bind_addr);
    assert (err == 0);
    s.context = context;
    s.responder = responder;
    return s;
}

// Recieves a new request from client connections
int receive(void* responder)
{
    int len = 10;
    char buffer[len];
    zmq_recv(responder, buffer, len, 0);
    printf("Received %s from client\n", buffer);
    sleep(1);
    zmq_send(responder, "ACK", 3, 0);
    return 0;
}