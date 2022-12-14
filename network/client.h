// Client header file using 0MQ
#include "packet.h"
#include <czmq.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef CLIENT_H
#define CLIENT_H

// Structure containing the information for 0mq client connection
struct client {
  void *context;
  zsock_t *requester;
};

// Initializes and returns a new client connection
struct client *new_client(const char *, const char *);

// Sends a new message using the given client connection, returns the server's
// response
// TODO: Currently does not handle chunked messages
void *send_msg(zsock_t *, int, PacketType, void *, uint32_t);
#endif
