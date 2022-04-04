#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "client.h"
#include "server.h"

int main(void)
{
    int i;
    const char* port = "55000";
    const char* address = "127.0.0.1";
    struct client* c = new_client(port, address);
    char* ping = "Ping";
    send_msg(c->requester, ping, 4);
    return 0;
}