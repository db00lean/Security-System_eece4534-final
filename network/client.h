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
  int cam_id;
};

// Initializes and returns a new client connection
struct client *new_client(const char *, const char *, int);

// Sends a new message using the given client connection, does not wait for a response from the server
void send_msg(zsock_t *, int, PacketType, void *, uint32_t);
#endif
