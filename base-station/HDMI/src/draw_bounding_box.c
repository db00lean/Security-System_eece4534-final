/*
This file can draw a bounding box for a given x,y start and x,y length. 
Any color hex code can be used in format 0xAABBCC where AA is red, BB is green, and CC is blue
Writes box pixels to frame buffer /dev/fb0
*/
#include "../inc/draw_bounding_box.h"
#include "../inc/DRM_user.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

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

void draw_circle_outline(int x_center, int y_center, int radius, unsigned int color) {
    //first half of outline
    for (int x = x_center - radius; x <= x_center + radius; x++) {
        int y = y_center + (int)sqrt(pow(radius, 2) - pow((x-x_center), 2));
        draw_pixel(x, y, color);
    }
    //second half of outline
    for (int x = x_center - radius; x <= x_center + radius; x++) {
        int y = y_center - (int)sqrt(pow(radius, 2) - pow((x-x_center), 2));
        draw_pixel(x, y, color);
    }
}

void draw_circle_filled(int x_center, int y_center, int radius, unsigned int color) {
    //store current state in array
    int state[radius*2][radius*2];
    for (int i = 0; i<(radius*2); i++) {
        for (int j = 0; j<(radius*2); j++) {
            state[i][j] = 0;
        }
    }

    //first half of outline
    for (int x = x_center - radius; x <= x_center + radius; x++) {
        int y = y_center + (int)sqrt(pow(radius, 2) - pow((x-x_center), 2));
        draw_pixel(x, y, color);
        state[x-radius][y-radius] = 1;
    }
    //second half of outline
    for (int x = x_center - radius; x <= x_center + radius; x++) {
        int y = y_center - (int)sqrt(pow(radius, 2) - pow((x-x_center), 2));
        draw_pixel(x, y, color);
        state[x-radius][y-radius] = 1;
    }
    //fill in circle
    for (int x = x_center - radius; x <= x_center + radius; x++) {
        for (int y = y_center - radius; y<= y_center + radius; y++) {
            if (state[x][y] == 1) {
                y++;
                while(state[x][y] == 0) {
                    state[x][y] = 1;
                    y++;
                }
            }
        }
    }
}

void draw_rectangle_filled(int x_start, int y_start, int x_len, int y_len, unsigned int color) {
    for (int x = x_start; x<x_len; x++) {
        for (int y = y_start; y<y_len; y++) {
            draw_pixel(x, y, color);
        }
    }
}
