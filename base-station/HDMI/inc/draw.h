/*
This is the header file for drawing shapes and text on the HDMI displau
*/

#ifndef __DRAW_H__
#define __DRAW_H__

#include "../inc/DRM_user.h"

#include <math.h>
#include <stdbool.h>
#include <string.h>

#define SHAPE_RECT 0x0
#define SHAPE_CIR 0x1

#define JUSTIFY_L 0x0
#define JUSTIFY_C 0x1
#define JUSTIFY_R 0x2

#define JUSTIFY_T 0x0
#define JUSTIFY_B 0x2

extern char fontdata_8x8[];

struct textObj
{
    uint32_t textColor;
    bool hasBackground;
    uint32_t backColor;
    char msg[50];
};

struct shapeObj
{
	uint8_t type;
	uint16_t dim1;
	uint16_t dim2;
	bool hasFill;
	uint32_t fillColor;
	bool hasOutline;
	uint32_t outlineColor;
	uint16_t lineWidth;
};

struct shapeObj * create_circle(uint16_t r, bool hasFill, uint32_t fill, bool hasOutline, uint32_t outline);

struct shapeObj * create_rect(uint16_t l, uint16_t h, bool hasFill, uint32_t fill, bool hasOutline, uint32_t outline);

void draw_shape(uint16_t x, uint16_t y, struct shapeObj * shape, uint8_t justify_h, uint8_t jusitfy_v);

void draw_text(int x_start, int y_start, char* str, uint32_t color, int scale);

#endif