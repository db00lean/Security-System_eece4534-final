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

    unsigned int color = 0xffffff;
    int x, y;

    //draw top line
    x = x_start;
    y = y_start;
    for (x = x_start; x < x_len + x_start; x++) {
        draw_pixel(&fb_info, x, y, color);
    }

    //draw bottom line
    x = x_start;
    y = y_start + y_len;
    for (x = x_start; x < x_len + x_start; x++) {
        draw_pixel(&fb_info, x, y, color);
    }

    //draw left line
    x = x_start;
    y = y_start;
    for (y = y_start; y < y_len + y_start; y++) {
        draw_pixel(&fb_info, x, y, color);
    }

    //draw right line
    x = x_start + x_len;
    y = y_start;
    for (y = y_start; y < y_len + y_start; y++) {
        draw_pixel(&fb_info, x, y, color);
    }

}



int main() {

    int x_start = 0;
    int y_start = 0;
    int x_len = 10;
    int y_len = 10;

    //read values x_start, y_start and x_len and y_len from sysfs
    FILE *fp = fopen("/sys/class/axitimer/axitimer0/control/state", "r");
    char* read_buffer;
    size_t res = fread(read_buffer, 1, 1, fp);
    //convert to int
    x_start = atoi(read_buffer);
    fclose(fp);


    fp = fopen("/sys/class/axitimer/axitimer0/control/period", "r");
    res = fread(read_buffer, 1, 1, fp);
    //convert to int
    y_start = atoi(read_buffer);
    fclose(fp);

    fp = fopen("/sys/class/axitimer/axitimer0/control/duty", "r");
    res = fread(read_buffer, 1, 1, fp);
    //convert to int
    x_len = atoi(read_buffer);
    y_len = atoi(read_buffer);
    fclose(fp);

    //draw bounding box
    draw_boundingbox(x_start, y_start, x_len, y_len);


    return 0;
}