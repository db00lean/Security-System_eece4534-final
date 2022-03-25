/* 
Matthew Downing
Inch Pebble 1

uses drawpixel library to create a rectangle given user input
*/

#include <stdio.h>
#include <linux/fb.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <fcntl.h>

//#include "drawpixel.h"

//static struct fb_info fb_info;
static struct fb_var_screeninfo screeninfo;

void fb_open(){
    int fd;
	fd = open("/dev/fb0", O_RDWR);

    if (fd == -1){
        printf("bad file\n");
    }
	ioctl(fd, FBIOGET_VSCREENINFO, &screeninfo);
}



// void draw_boundingbox(int x_start, int y_start, int x_len, int y_len) {
//     //origin is top left of rectangle (x_start, y_start)

//     //white
//     // unsigned int color = 0xffffff;
//     unsigned int color = 0xff0000;
//     int x, y;

//     //draw top line
//     x = x_start;
//     y = y_start;
//     for (x = x_start; x < x_len + x_start; x++) {
//         draw_pixel(&fb_info, x, y, color);
//     }

//     //draw bottom line
//     x = x_start;
//     y = y_start + y_len;
//     for (x = x_start; x < x_len + x_start; x++) {
//         draw_pixel(&fb_info, x, y, color);
//     }

//     //draw left line
//     x = x_start;
//     y = y_start;
//     for (y = y_start; y < y_len + y_start; y++) {
//         draw_pixel(&fb_info, x, y, color);
//     }

//     //draw right line
//     x = x_start + x_len;
//     y = y_start;
//     for (y = y_start; y < y_len + y_start; y++) {
//         draw_pixel(&fb_info, x, y, color);
//     }

// }



int main() {

    int x_start = 100;
    int y_start = 100;
    int x_len = 100;
    int y_len = 100;

    //draw bounding box
    fb_open();

    uint32_t xRes = screeninfo.xres;
    uint32_t yRes = screeninfo.yres;

    uint32_t xRes_virtual = screeninfo.xres_virtual;
    uint32_t yRes_virtual = screeninfo.yres_virtual;

    uint32_t bits_per_pixel = screeninfo.bits_per_pixel;


    printf("xres: %d, yres: %d\n", xRes, yRes);
    printf("xres: %d, yres: %d\n", xRes_virtual, yRes_virtual);
    printf("bits per pixel: %d\n", bits_per_pixel);


    // draw_boundingbox(x_start, y_start, x_len, y_len);


    return 0;
}