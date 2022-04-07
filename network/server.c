#include <czmq.h>
#include "server.h"
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

// Initializes a new server connection 
struct server* new_server(const char* port)
{
    int err;
    struct server* s = malloc(sizeof(server));
    char bind_addr[19];
    // Initialize the context and the requester socket
    sprintf(bind_addr, "tcp://*:%s", port);
    printf("Server bind address is %s\n", bind_addr);
    s->context = zmq_ctx_new();
    s->responder = zmq_socket(s->context, ZMQ_REP);
    // Bind requester to socket using the given server information
    err = zmq_bind(s->responder, bind_addr);
    assert (err == 0);
    return s;
}

// Recieves a new request from client connections, this is currently a BLOCKING call
// This will be changed in a future issue
int receive_msg(zsock_t* responder)
{
    int size;
    int max = 256;
    zmq_msg_t msg;
    int rc = zmq_msg_init(&msg);
    if (rc == -1) 
    {
        fprintf(stderr, "Error initializing message structure\n");
    }
    printf("Before receive call\n");
    size = zmq_msg_recv(&msg, responder, 0);
    if (size == -1)
    {
        fprintf(stderr, "Error receiving messages: %s\n", strerror(errno));
        return -1;
    }
    printf("Received %s from client\n", (char*)zmq_msg_data(&msg));
    sleep(1);
    size = zmq_send(responder, "ACK", 3, 0);
    return size;
}