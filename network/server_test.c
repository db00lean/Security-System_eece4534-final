#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "client.h"
#include "server.h"

// Creates a simple server and receives a single message from a client 
int main(void)
{
    received_message* msg;
    const char* port = "55000";
    struct server* s = new_server(port);
    struct client_hello* ch;
    msg = receive_msg(s->register_s);
    ch = (struct client_hello*)msg->data;
    if (msg->type == CLIENT_HELLO)
    {
        printf("Processing new client\n");
        register_client(s, ch->cam_id);
        printf("Registered new client\n: %i", s->clients[ch->cam_id]->port);
    }
    printf("Received message\n");
    printf("Camera id: %i\n", msg->cam_id);
    printf("Data type: %i\n", msg->type);
    printf("Data length: %i\n", msg->len);
    free(s);
    return 0;
}