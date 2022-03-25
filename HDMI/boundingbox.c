/*
This file can draw a bounding box for a given x,y start and x,y length
Writes box pixels to frame buffer /dev/fb0
*/
#include <stdio.h>
#include <linux/fb.h>
#include <stdint.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fcntl.h>

//static struct fb_info fb_info;
static struct fb_var_screeninfo var_screeninfo;
static struct fb_fix_screeninfo fix_screeninfo;

static void *fbMemPtr; 

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

static void draw_pixel(int x, int y, uint32_t color)
{
    uint32_t *pixelPtr;
    pixelPtr = fbMemPtr;
    pixelPtr += fix_screeninfo.line_length * y;
    pixelPtr += x;

    // printf("fbMemPtr: 0x%x\n", fbMemPtr);
    // printf("pixelPtr: 0x%x\n", pixelPtr);

    // printf("x %d, y %d, color 0x%x\n", x, y, color);
    *pixelPtr = color;
}


void draw_boundingbox(int x_start, int y_start, int x_len, int y_len) {
    //origin is top left of rectangle (x_start, y_start)

    //white
    // unsigned int color = 0xffffff;
    unsigned int color = 0x00ff00;
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



int main() {

    int x_start = 100;
    int y_start = 100;
    int x_len = 100;
    int y_len = 100;

    //draw bounding box
    fb_open();

    uint32_t xRes = var_screeninfo.xres;
    uint32_t yRes = var_screeninfo.yres;

    uint32_t xRes_virtual = var_screeninfo.xres_virtual;
    uint32_t yRes_virtual = var_screeninfo.yres_virtual;

    uint32_t bits_per_pixel = var_screeninfo.bits_per_pixel;


    printf("xres: %d, yres: %d\n", xRes, yRes);
    printf("xres: %d, yres: %d\n", xRes_virtual, yRes_virtual);
    printf("bits per pixel: %d\n", bits_per_pixel);


    draw_boundingbox(x_start, y_start, x_len, y_len);


    return 0;
}