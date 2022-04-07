/*
This file can draw a bounding box for a given x,y start and x,y length
Writes box pixels to frame buffer /dev/fb0
*/
#include "draw_bounding_box.h"

#include <stdio.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>

void fb_open(){
    int fd;
	fd = open("/dev/fb0", O_RDWR);

    if (fd == -1){
        printf("bad file\n");
    }
	ioctl(fd, FBIOGET_VSCREENINFO, &var_screeninfo);
	ioctl(fd, FBIOGET_FSCREENINFO, &fix_screeninfo);
    
    fbMemPtr = mmap(0,
			var_screeninfo.yres_virtual * fix_screeninfo.line_length,
			PROT_WRITE | PROT_READ,
			MAP_SHARED, fd, 0);
    printf("fbmem pointer initial %x\n", fbMemPtr);

}

void draw_pixel(int x, int y, uint32_t color)
{
    uint32_t *pixelPtr;
    pixelPtr = fbMemPtr;
    pixelPtr += (fix_screeninfo.line_length / 4) * y;
    pixelPtr += x;

    *pixelPtr = color;
}

void draw_boundingbox(int x_start, int y_start, int x_len, int y_len, unsigned int color) {
    //origin is top left of rectangle (x_start, y_start)
    int x, y;

    //draw top line
    x = x_start;
    y = y_start;
    for (x = x_start; x < x_len + x_start; x++) {
        draw_pixel(x, y, color);
    }

    //draw bottom line
    x = x_start;
    y = y_start + y_len;
    for (x = x_start; x < x_len + x_start; x++) {
        draw_pixel(x, y, color);
    }

    //draw left line
    x = x_start;
    y = y_start;
    for (y = y_start; y < y_len + y_start; y++) {
        draw_pixel(x, y, color);
    }

    //draw right line
    x = x_start + x_len;
    y = y_start;
    for (y = y_start; y < y_len + y_start; y++) {
        draw_pixel(x, y, color);
    }

}