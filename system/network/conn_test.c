#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include "server.h"

// Simple test function, sends a ping and expects a pong to print back
int main(void)
{
    const char* port = "5500";
    const char* address = "localhost";
    char* ping = "Ping";
    struct server s = new_server(port);
    struct client c = new_client(port, address);

    send(c.requester, ping, 4);
    receive(s.responder);
    return 0;
}