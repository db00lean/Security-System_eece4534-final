#include "packet.h"
#include <stdlib.h>

// Returns the packet header data from the zmq message
packet_header * parse_packet_header(zmq_msg_t* msg) {
    void * packet_data;
    struct packet_header * ph;
    packet_data = zmq_msg_data(msg);
    ph = malloc(sizeof(packet_header));
    ph->cam_id = *packet_data;
    ph->type = *(packet_data + sizeof(int));
    ph->len = *(packet_data + sizeof(int) + sizeof(PacketType));
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
packet_header * build_packet(int cam_id, PacketType type, int data_len);
    struct packet * p;
    p = malloc(sizeof(packet));
    p->cam_id = cam_id;
    p->type = type;
    p->len = data_len;
    return p;
}

// Frees the memory in a packet
int free_packet(packet* p) {
    free(p);
}