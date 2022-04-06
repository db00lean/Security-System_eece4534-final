#include <czmq.h>

#ifndef PACKET_H
#define PACKET_H

typedef enum PacketType {
    CV_DATA
} PacketType;

typedef struct packet_header {
    int cam_id;
    PacketType type;
    static int len;
} packet_header;

// Returns the packet header data from the zmq message
packet_header * parse_packet(zmq_msg_t msg);

// Creates a packet with the provided details
packet_header * build_packet(int cam_id, PacketType type, int data_len);

// Frees the packet memory
int free_packet(packet_header* p);

#endif