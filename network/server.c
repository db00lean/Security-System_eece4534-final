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
received_message* receive_msg(zsock_t* responder)
{
    int size;
    // TODO: Will need to implement a maximum packet size if data becomes too large
    int max = 256;
    // The zeromq message to store bytes read off socket in
    zmq_msg_t zmsg;
    // The message structure to store the data parsed in and return to the caller
    received_message* msg = malloc(sizeof(received_message));
    // Stores the packet header data
    packet_header* header;
    // Stores the packet data
    void* data;

    int rc = zmq_msg_init(&zmsg);
    if (rc == -1) 
    {
        fprintf(stderr, "Error initializing message structure\n");
    }
    printf("Before receive call\n");
    size = zmq_msg_recv(&zmsg, responder, 0);
    if (size == -1)
    {
        // TODO: Check error code and see if a retry is necessary 
        fprintf(stderr, "Error receiving messages: %s\n", strerror(errno));
        return NULL;
    }
    printf("Received message from client\n");
    // Parse out header and data, store in received_message structure to return to caller
    header = parse_packet_header(&zmsg);
    data = parse_packet_data(&zmsg);
    msg->cam_id = header->cam_id;
    msg->type = header->type;
    msg->len = header->len;
    // Copy data to new buffer to return to caller
    msg->data = malloc(header->len);
    memcpy(msg->data, data, header->len);
    // TODO: send an ack to the client (probably to change in the future)
    // size = zmq_send(responder, "ACK", 3, 0);
    // Free memory
    free_packet(header);
    // Return message
    return msg;
}