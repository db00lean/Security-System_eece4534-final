#include <czmq.h>
#include "server.h"
#include "hello.h"
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

// Initializes a new server connection 
struct server* new_server(const char* port)
{
    int err;
    struct server* s = malloc(sizeof(server));
    char bind_addr[19];
    // Initialize the context and the requester socket
    sprintf(bind_addr, "tcp://*:%s", port);
    sprintf(s->reg_port, "%s", port);
    printf("Server registration address is %s\n", bind_addr);
    s->context = zmq_ctx_new();
    s->register_s = zmq_socket(s->context, ZMQ_REP);
    s->num_clients = 0;
    // Bind to registration port and begin listening for new client connections
    err = zmq_bind(s->register_s, bind_addr);
    assert (err == 0);
    return s;
}

// Recieves a new request from client connections, this is currently a BLOCKING call
// This will be changed in a future issue
received_message* receive_msg(zsock_t* responder)
{
    int size;
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

// Registers a new client and saves its state to the given server
int register_client(struct server* s, int cam_id)
{
    received_message* msg;
    struct client_conn* client = (struct client_conn*)malloc(sizeof(client_conn));
    int reg_port = atoi(s->reg_port);
    int bind_port = reg_port; 
    char client_port[5];
    char bind_addr[19];
    int lower = 10000;
    int upper = 65535;
    
    // Only support MAX_NUM clients at a time
    // TODO: Add a way to check for existing taken ports
    if (s->num_clients >= MAX_CLIENTS) 
    {
        return -1;
    }
    // Receive new client registration on port (assume caller has checked that there is one to receive)
    msg = receive_msg(s->register_s);
    // TODO: Verify that we received a register packet, otherwise throw it away
    // TODO: Some magic checking to make sure that the camera id they request here is valid, crypto stuff if time
    // For now assume they are who they say they are and setup the connection
    // Generate a random port that isn't the registration port
    while(bind_port != reg_port)
    {
        bind_port = rand() % ((upper - lower + 1) + lower);
    }
    sprintf(client_port, "%i", bind_port);
    printf("Generated port number for new client is %s\n", client_port);
    // Create a new zeromq pair socket on that port number
    // Initialize the context and the requester socket
    sprintf(bind_addr, "tcp://*:%s", client_port);
    printf("Server listening for new client on %s\n", bind_addr);
    s->clients[msg->cam_id] = client;
    s->clients[msg->cam_id]->sock = zmq_socket(s->context, ZMQ_PAIR);
    s->clients[msg->cam_id]->port = bind_port;
    int err = zmq_bind(s->clients[msg->cam_id], bind_addr);
    // Make sure we've successfully bound to that socket
    assert (err == 0);
    // Send back 0 indicating success
    return 0;
}

// Sends a new request to the previously initialized 0mq client, timeout will be adjusted upon discussing with other sysman members.
void send_client_msg(struct server* s, int cam_id, PacketType type, void* buff, uint32_t len)
{
    //char* response = (char*) malloc(SERVER_RESPONSE_LENGTH);
    int msg_len;
    packet_header* p;
    zmq_msg_t msg;
    zsock_t* client_socket = s->clients[cam_id]->sock;
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
    printf("Sending message to client\n");
    rc = zmq_msg_send(&msg, (void*)client_socket, ZMQ_DONTWAIT);
    if (rc == -1)
    {
        fprintf(stderr, "Error sending message to client: %s\n", strerror(errno));    
    }
    assert(rc == msg_len);
    return;
}

// Sends a server hello to the requested client on their registered port
void send_server_hello(struct server* s, int cam_id)
{
    struct server_hello sh;
    sh.cam_id = cam_id;
    sh.port = s->clients[cam_id]->port;
    send_client_msg(s, cam_id, SERVER_HELLO, (void*)&sh, sizeof(server_hello));
}