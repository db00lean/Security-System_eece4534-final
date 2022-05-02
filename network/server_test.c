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
    msg = receive_msg(s->register_s);
    if (msg->type == CLIENT_HELLO)
    {
        printf("Processing new client\n");
        register_client(s);
    }
    printf("Received message\n");
    printf("Camera id: %i\n", msg->cam_id);
    printf("Data type: %i\n", msg->type);
    printf("Data length: %i\n", msg->len);
    free(s);
    return 0;
}