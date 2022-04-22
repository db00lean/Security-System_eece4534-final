/*
Read in data from cv struct and output bounding boxes accordingly
*/

#include "draw_bounding_box.h"
#include "cv_structs.h"

#include <stdio.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>


int main() {
    //Using cv data struct format
    struct cv_data cvdata1;
    //Fill with data for now
    for (int i = 0; i<MAX_B_BOXES; i++) {
        cvdata1.num_bbox++; //increase number of boxes
        //fill in coordinates
        cvdata1.box_data[i].x_coord = 40 + (i * 60);
        cvdata1.box_data[i].y_coord = 40;
        cvdata1.box_data[i].x_len = 50;
        cvdata1.box_data[i].y_len = 50;
    }

    fb_open();
    //for each box, draw a rectangle bounding box
    for (int i = 0; i<cvdata1.num_bbox; i++) {
        unsigned int color = 0xFF00FF;
        //printf("draw pixel %d %d %d %d %x\n", cvdata1.box_data->x_coord, cvdata1.box_data->y_coord, cvdata1.box_data->x_len, cvdata1.box_data->y_len, color);
        draw_boundingbox(cvdata1.box_data[i].x_coord, cvdata1.box_data[i].y_coord, cvdata1.box_data[i].x_len, cvdata1.box_data[i].y_len, color);
    }

}

