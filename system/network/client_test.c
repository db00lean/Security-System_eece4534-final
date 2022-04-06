#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "client.h"
#include "server.h"

// Creates a single client and sends a "ping" message to the server program, then waits for an ACK
int main(void)
{
    const char* port = "55000";
    const char* address = "127.0.0.1";
    struct client* c = new_client(port, address);
    char* ping = "Ping";
    send_msg(c->requester, ping, 4);
    free(c);
    return 0;
}