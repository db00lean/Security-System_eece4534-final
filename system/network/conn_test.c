#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "client.h"
#include "server.h"

#define NTHREADS 2
// Simple test function, sends a ping and expects a pong to print back
void *server_thread(void* responder)
{
    receive_msg((zsock_t*)responder);
    return;
}

void *client_thread(void* requester)
{
    char* ping = "Ping";
    send_msg((zsock_t*)requester, ping, 4);
    return;
}

int main(void)
{
    int i;
    pthread_t thread_id[NTHREADS];
    const char* port = "5500";
    const char* address = "127.0.0.1";
    struct server* s = new_server(port);
    struct client* c = new_client(port, address);
    pthread_create(&thread_id[1], NULL, server_thread, s->responder);
    pthread_create(&thread_id[0], NULL, client_thread, (void*)c->requester);
    for (i = 0; i < NTHREADS; i++) 
    {
        pthread_join(thread_id[i], NULL);
    }
    return 0;
}