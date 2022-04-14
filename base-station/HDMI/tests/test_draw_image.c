/*
Read in data from Connor's image struct and output to screen

*/

#include "draw_bounding_box.h"
#include "imagelib.h"

#include <stdio.h>
#include <linux/fb.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>

#define PIXEL(x,y) ((y * IMG_W * 3) + (x * 3))


int main()
{
    // Using image struct from imagelib.h and fill with data for now
    int IMG_W = 1920;
    int IMG_H = 1080;
    struct image *img = create_image(IMG_W, IMG_H);

    //taken from Connor's image-out.c
    // _very_ quick and dirty example
    // don't replicate this
    // it's just to get an image.
    /*
    for (int ii = 0; ii < IMG_H; ii++)
    { // rows
        for (int jj = 0; jj < IMG_W; jj++)
        { // columns
            //  color within pixel: +0,1,2
            img->buf[PIXEL(jj, ii) + 0] = 255 * jj / 3 / IMG_W;
            img->buf[PIXEL(jj, ii) + 1] = 0;
            img->buf[PIXEL(jj, ii) + 2] = 255 * ii / IMG_H;
        }
    }
    */
   for (int i = 0; i<IMG_H; i++) {
       for (int j = 0; j<IMG_W; j++) {
           img->buf[i + j + 0] = i % 255;
           img->buf[i + j + 1] = 50;
           img->buf[i + j + 2] = j % 255;
       }   
    }



    fb_open();
    //draw image to screen using draw pixel
    // for each box, draw a rectangle bounding box
    for (int x = 0; x < IMG_W; x++) {
        for (int y = 0; y < IMG_H; y++) {
            unsigned int color = (img->buf[x + y] << 16) | (img->buf[x + y + 1] << 8) | (img->buf[x + y + 2] << 0);
            printf("color %x ", color);
            printf("%d %d %d\n", img->buf[x + y] << 16), (img->buf[x + y + 1] << 8), (img->buf[x + y + 2] << 0);
            draw_pixel(x,y,color);
        }
    }
}
