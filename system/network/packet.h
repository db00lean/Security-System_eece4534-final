#ifndef PACKET_H
#define PACKET_H

enum PacketType {CV_DATA, COORD_DATA};

typedef struct packet {
    int cam_id;
    PacketType type;
    int len;
    void * data;
} packet;

// Returns type of packet data
PacketType get_packet_type(packet p);

// Returns camera ID from packet
int get_cam_id(packet p);

// Returns the data in the form of a void *
// ** The data is copied into a separate memory space so the packet can be free**
void * get_packet_data(packet p); //memcpy

// Creates a packet with the provided details
packet * build_packet(int cam_id, int data_len, void * data);

// Frees the memory in a packet
int free_packet(packet* p);

#endif