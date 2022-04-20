/*
This is the header file for drawing bounding boxes on the HDMI display
*/

#ifndef __DRAW_BOUNDING_BOX__
#define __DRAW_BOUNDING_BOX__

#include <stdint.h>

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


/*
This function draws a circle outline at a given x,y coordinate center location for a given radius with a given color
Arguments:
int x_center: center x coordinate of circle
int y_center: center y coordinate of circle
int radius: radius of circle
unsigned int color: the value of the color in R,G,B format (with each color being one byte)
No return
*/
void draw_circle_outline(int x_center, int y_center, int radius, unsigned int color);

/*
This function draws a circle at a given x,y coordinate center location for a given radius with a given color
Arguments:
int x_center: center x coordinate of circle
int y_center: center y coordinate of circle
int radius: radius of circle
unsigned int color: the value of the color in R,G,B format (with each color being one byte)
No return
*/
void draw_circle_filled(int x_center, int y_center, int radius, unsigned int color);

/*
This function draws a filled in rectangle at a given x,y coordinate starting location for a given x,y length with a given color
Arguments:
int x_start: top left x coordinate to start rectangle
int y_start: top left y coordinate to start rectangle
int x_len: distance to draw in the horizontal x direction
int y_len: distance to draw in the vertical y direction
unsigned int color: the value of the color in R,G,B format (with each color being one byte)
No return
*/
void draw_rectangle_filled(int x_start, int y_start, int x_len, int y_len, unsigned int color);

#endif