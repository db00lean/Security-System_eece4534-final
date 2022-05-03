#include "client.h"
#include "packet.h"
#include "hello.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <czmq.h>
#define REQUEST_TIMEOUT 2500 //milliseconds
#define SERVER_RESPONSE_LENGTH 255

// Recieves a new request from client connections, this is currently a BLOCKING call
// This will be changed in a future issue
received_message* receive_server_msg(zsock_t* responder)
{
    int size;
    // The zeromq message to store bytes read off socket in
    zmq_msg_t zmsg;
    // The message structure to store the data parsed in and return to the caller
    received_message* msg = (received_message*)malloc(sizeof(received_message));
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
    printf("Received message from server\n");
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

// Sends a new request to the previously initialized 0mq client, this currently polls periodically for an acknowledgement 
// from the server and exits once received, tiemout will be adjusted upon discussing with other sysman members.
void send_msg(zsock_t* requester, int cam_id, PacketType type, void* buff, uint32_t len)
{
    //char* response = (char*) malloc(SERVER_RESPONSE_LENGTH);
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
    return;
}

// Returns the socket the server has asked us to bind to
int send_client_hello(int cam_id, zsock_t* registration_port, struct client* c)
{
    struct client_hello ch;
    struct received_message msg;
    struct server_hello* sh;
    ch.cam_id = cam_id;
    
    printf("Sending client hello to server\n");
    send_msg(registration_port, cam_id, CLIENT_HELLO, (void*)&ch, sizeof(ch));
    msg = *receive_server_msg(registration_port);
    sh = (struct server_hello*)msg.data;
    // if they assigned us a different camera id store that in the client
    if (sh->cam_id != c->cam_id)
    {
        c->cam_id = sh->cam_id;
    }
    printf("Server wants us to connect to port %i\n", sh->port);
    return sh->port;
}

// Initializes and returns a new client connection
struct client* new_client(const char* server_port, const char* server_address, int cam_id)
{
    int err;
    zsock_t* registration_port;
    int client_port_num;
    struct client* c = (struct client*) malloc(sizeof(struct client));
    char bind_addr[25]; 
    // Initialize the context and the registration socket
    sprintf(bind_addr, "tcp://%s:%s", server_address, server_port);
    printf("Client bind address is %s\n", bind_addr);
    c->context = zmq_ctx_new();
    // Create a client connection to the registration socket on the server side
    registration_port = (zsock_t*)zmq_socket(c->context, ZMQ_REQ);
    if (!registration_port) {
        fprintf(stderr, "Error creating connection to registration socket on server: %s\n", strerror(errno));    
    }
    assert(registration_port != 0);
    err = zmq_connect(registration_port, bind_addr);
    if (err == -1)
    {
        fprintf(stderr, "Error connecting to server on registration socket: %s\n", strerror(errno)); 
    }
    assert(err == 0);
    client_port_num = send_client_hello(cam_id, registration_port, c);
    zmq_close((void*)registration_port);
    // Initialize the new client socket
    sprintf(bind_addr, "tcp://%s:%i", server_address, client_port_num);
    printf("Client bind address is now %s\n", bind_addr);
    c->requester = (zsock_t*)zmq_socket(c->context, ZMQ_PAIR);
     if (!c->requester) {
        fprintf(stderr, "Error creating connection to pair socket on server: %s\n", strerror(errno));    
    }
    assert(c->requester != 0);
    err = zmq_connect(c->requester, bind_addr);
    if (err == -1)
    {
        fprintf(stderr, "Error connecting to server on pair socket: %s\n", strerror(errno)); 
    }
    assert(err == 0);
    return c;
}