#ifndef PACKET_H
#define PACKET_H

enum PacketType {CV_DATA, COORD_DATA};

struct packet {
    int cam_id;
    PacketType type;
    int len;
    void * data;
};

#endif