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
    const char* port = "55000";
    struct server* s = new_server(port);
    receive_msg(s->responder);
    free(s);
    return 0;
}