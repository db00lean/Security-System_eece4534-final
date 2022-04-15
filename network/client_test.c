#include <czmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "../common_headers/cv_structs.h"
#include "client.h"
#include "server.h"

// Creates a single client and sends a "ping" message to the server program, then waits for an ACK
int main(void)
{
    
    struct coordinate_data cd;
    struct cv_data cv;
    const char* port = "55000";
    const char* address = "127.0.0.1";
    struct client* c = new_client(port, address);

    while(1) {
        cd.x_coord = 5;
        cd.y_coord = 5;
        cd.x_len = 10;
        cd.y_len = 10;
        cv.num_bbox = 1;
        cv.box_data[0] = cd;
        send_msg(c->requester, 0, CV_DATA, (void*)&cv, sizeof(struct cv_data)+sizeof(struct coordinate_data));
        usleep(3000000);
        cd.x_coord = 50;
        cd.y_coord = 50;
        cd.x_len = 15;
        cd.y_len = 15;
        cv.num_bbox = 1;
        cv.box_data[0] = cd;
        send_msg(c->requester, 0, CV_DATA, (void*)&cv, sizeof(struct cv_data)+sizeof(struct coordinate_data));
        usleep(3000000);

    }
    
    
    free(c);
    return 0;
}