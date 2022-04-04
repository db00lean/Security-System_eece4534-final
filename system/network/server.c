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
    char bind_addr[13];
    // Initialize the context and the requester socket
    sprintf(bind_addr, "tcp://*:%s", port);
    printf("Server bind address is %s\n", bind_addr);
    void* context = zmq_ctx_new();
    zsock_t* responder = zmq_socket(context, ZMQ_REP);
    // Bind requester to socket using the given server information
    err = zmq_bind(responder, bind_addr);
    assert (err == 0);
    s->context = context;
    s->responder = responder;
    return s;
}

// Recieves a new request from client connections
int receive_msg(zsock_t* responder)
{
    int size;
    int max = 256;
    zmq_msg_t msg;
    char buffer[max];
    int rc = zmq_msg_init(&msg);
    printf("Before receive call\n");
    size = zmq_msg_recv(&msg, responder, ZMQ_DONTWAIT);
    if (size == -1)
    {
        fprintf(stderr, "Error receiving messages: %s\n", strerror(errno));
        return -1;
    }
    buffer[size < max ? size : max - 1] = '\0';
    printf("Received %s from client\n", buffer);
    sleep(1);
    size = zmq_send(responder, "ACK", 3, 0);     
    return size;
}