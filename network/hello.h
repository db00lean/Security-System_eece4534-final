#include <czmq.h>

#ifndef HELLO_H
#define HELLO_H

typedef struct server_hello {
    int cam_id;
    const char * port;
} server_hello;

typedef struct client_hello {
    int cam_id;
    const char * port;
} server_hello;



#endif