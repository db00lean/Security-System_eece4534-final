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
    struct server* s = new_server(port);
    while (1)
    {
        receive_msg(s->responder);
    }
    return 0;
}