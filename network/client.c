#include "client.h"
#include "packet.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <czmq.h>
#define REQUEST_TIMEOUT 2500 //milliseconds
#define SERVER_RESPONSE_LENGTH 255

// Initializes and returns a new client connection
struct client* new_client(const char* server_port, const char* server_address)
{
    int err; 
    struct client* c = (struct client*) malloc(sizeof(struct client));
    char bind_addr[32]; // @john Craffey I changed this from 20 to 22 because sprintf was giving me illegal hardware instruction with it at 20
    // Initialize the context and the requester socket
    sprintf(bind_addr, "tcp://%s:%s", server_address, server_port);
    printf("Client bind address is %s\n", bind_addr);
    c->context = zmq_ctx_new();
    // Bind requester to socket using the given server information
    c->requester = (zsock_t*) zmq_socket(c->context, ZMQ_REQ);
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

// Sends a new request to the previously initialized 0mq client, this currently polls periodically for an acknowledgement 
// from the server and exits once received, tiemout will be adjusted upon discussing with other sysman members.
void* send_msg(zsock_t* requester, int cam_id, PacketType type, void* buff, uint32_t len)
{
    char* response = (char*) malloc(SERVER_RESPONSE_LENGTH);
    int wait_reply = 1;
    int msg_len;
    packet_header* p;
    zmq_msg_t msg;

    // calculate total message length
    msg_len = sizeof(packet_header)+len;
    int rc = zmq_msg_init_size(&msg, msg_len);
    // Ensure message size was initialized properly
    assert(rc == 0);
    // Get packet header
    p = build_packet(cam_id, type, len);
    // Copy header into message
    memcpy(zmq_msg_data(&msg), p, sizeof(packet_header));
    // Copy data into message
    memcpy(zmq_msg_data(&msg)+sizeof(packet_header), buff, len);
    // Send message to server
    printf("Sending message to server\n");
    rc = zmq_msg_send(&msg, (void*)requester, ZMQ_DONTWAIT);
    if (rc == -1)
    {
        fprintf(stderr, "Error sending message to server: %s\n", strerror(errno));    
    }
    assert(rc == msg_len);
    // Wait for an ACK from the server
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
            int size = zmq_recv(requester, response, SERVER_RESPONSE_LENGTH, 0);
            if (size == -1)
            {
                printf("Received nothing from server\n");
                return NULL;
            }
            // TODO: this was placed as a bounds check to prevent overflows in memory. Must confer with other members to figure out what the 
            // max length of the server response should be. 
            if (size > SERVER_RESPONSE_LENGTH)
            {
                size = SERVER_RESPONSE_LENGTH;
            }
            // We have received a reply from the server and want to break out of the loop and return the pointer to its response.
            wait_reply = 0;
            printf("Received %s from server\n", response);
        }
    }
    return response;
}
