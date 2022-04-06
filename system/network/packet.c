#include "packet.h"
#include <stdlib.h>
#include <stdio.h>

// Returns the packet header data from the zmq message
packet_header * parse_packet_header(zmq_msg_t* msg) {
    void * packet_data;
    struct packet_header * ph;
    packet_data = zmq_msg_data(msg);
    ph = malloc(sizeof(packet_header));
    ph->cam_id = *(int*)packet_data;
    ph->type = *(PacketType*)(packet_data + sizeof(int));
    ph->len = *(int*)(packet_data + sizeof(int) + sizeof(PacketType));
    return ph;
}

// Returns the data from the zmq message
void * parse_packet_data(zmq_msg_t* msg) {
    void * packet_data;
    void * msg_data;
    packet_data = zmq_msg_data(msg);
    msg_data = packet_data + sizeof(packet_header);
    return msg_data;
}

// Creates a packet with the provided details
packet_header * build_packet(int cam_id, PacketType type, int data_len) {
    struct packet_header* p;
    p = malloc(sizeof(packet_header));
    p->cam_id = cam_id;
    p->type = type;
    p->len = data_len;
    return p;
}

// Frees the memory in a packet
int free_packet(packet_header* p) {
    free(p);
    return 0;
}