#include <czmq.h>

#ifndef HELLO_H
#define HELLO_H

typedef struct server_hello {
    int cam_id;
    int port;
} server_hello;

typedef struct client_hello {
    int cam_id;
    const char* ip_addr;
} client_hello;



#endif