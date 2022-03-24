/* 
Matthew Downing
Inch Pebble 1

uses drawpixel library to create a rectangle given user input
*/

#include <stdio.h>
#include "drawpixel.h"

static struct fb_info fb_info;


void draw_boundingbox(int x_start, int y_start, int x_len, int y_len) {
    //origin is top left of rectangle (x_start, y_start)
    //draws top line, then left, the right, then bottom line

    unsigned int color = 0xffffff;

    for (int x = x_start; x < xlen + x_start; x++) {
        draw_pixel(&fb_info, x, y, );
    }
}



int main() {

    int x_start, y_start, x_len, y_len;

    //read values x_start, y_start and x_len and y_len from sysfs
    

    //draw bounding box
    draw_bounding_box(x_start, y_start, x_len, y_len);


    return 0;
}