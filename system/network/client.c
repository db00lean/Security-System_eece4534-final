#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <czmq.h>
#define REQUEST_TIMEOUT 2500 //milliseconds
// Initializes and returns a new client connection
struct client* new_client(const char* server_port, const char* server_address)
{
    int err; 
    struct client* c = malloc(sizeof(client));
    char bind_addr[20];
    // Initialize the context and the requester socket
    sprintf(bind_addr, "tcp://%s:%s", server_address, server_port);
    printf("Client bind address is %s\n", bind_addr);
    c->context = zmq_ctx_new();
    // Bind requester to socket using the given server information
    c->requester = zmq_socket(c->context, ZMQ_REQ);
    if (!c->requester) {
        fprintf(stderr, "Error creating socket: %s\n", strerror(errno));    
    }
    assert(c->requester != 0);
    err = zmq_connect(c->requester, bind_addr);
    if (err == -1)
    {
        fprintf(stderr, "Error connecting to server: %s\n", strerror(errno)); 
    }
    assert(err == 0);
    return c;
}

// Sends a new request to the previously initialized 0mq client
int send_msg(zsock_t* requester, char* buff, uint32_t len)
{
    //zmq_msg_t msg;
    char recv[256];
    int wait_reply = 1;
    zmq_msg_t msg;

    int rc = zmq_msg_init_size(&msg, len);
    assert(rc == 0);
    memcpy(zmq_msg_data(&msg), buff, len);
    printf("Sending %s to server\n", buff);
    rc = zmq_msg_send(&msg, (void*)requester, ZMQ_DONTWAIT);
    if (rc == -1)
    {
        fprintf(stderr, "Error sending message to server: %s\n", strerror(errno));    
    }
    assert(rc == len);
    while(wait_reply)
    {
        printf("Waiting for reply from server\n");
        zmq_pollitem_t items[] = {{zsock_resolve(requester), 0, ZMQ_POLLIN, 0}};
        printf("After polling \n");
        int rc = zmq_poll(items, 1, REQUEST_TIMEOUT * ZMQ_POLL_MSEC);
        printf("Polling complete\n");
        if (rc == -1) 
        {
            break;
        }
        if(items[0].revents & ZMQ_POLLIN)
        {
            int size = zmq_recv(requester, recv, 255, 0);
            if (size == -1)
            {
                printf("Received nothing from server\n");
                break;
            }
            if (size > 255)
            {
                size = 255;
            }
            recv[size] = '\0';
            printf("Received %s from server\n", recv);
        }
    }
    return 0;
}