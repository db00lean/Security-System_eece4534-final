/*
This is the header file for drawing bounding boxes on the HDMI display
*/

#ifndef __DRAW_BOUNDING_BOX__
#define __DRAW_BOUNDING_BOX__

#include <stdint.h>


static struct fb_var_screeninfo var_screeninfo;
static struct fb_fix_screeninfo fix_screeninfo;

static void *fbMemPtr; 

/*
This function opens the frame buffer at /dev/fb0 and allocates memory appropriately
No arguments needed
No return
*/
void fb_open();

/*
This function draws the pixel as a specified x,y coordinate with a given color
Arguments:
int x: the value of the x coordinate on the screen
int y: the value of the y coordinate on the screen
uint32_t color: the value of the color in R,G,B format (with each color being one byte)
No return
*/
void draw_pixel(int x, int y, uint32_t color);

/*
This function draws a bounding box at a given x,y coordinate starting location for a given x,y length with a given color
Arguments:
int x_start: top left x coordinate to start rectangle
int y_start: top left y coordinate to start rectangle
int x_len: distance to draw in the horizontal x direction
int y_len: distance to draw in the vertical y direction
unsigned int color: the value of the color in R,G,B format (with each color being one byte)
No return
*/
void draw_boundingbox(int x_start, int y_start, int x_len, int y_len, unsigned int color);


#endif