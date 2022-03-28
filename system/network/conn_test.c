#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include "server.h"

// Simple test function, sends a ping and expects a pong to print back
int main(void)
{
    struct server s = new_server("5500");
    struct client c = new_client("5500", "localhost");

    char ping = "Ping";

    send(c.requester, &ping);
    receive(s.responder);
    return 0;
}