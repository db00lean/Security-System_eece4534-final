#include "../inc/draw.h"

static void draw_rect(uint16_t x_start, uint16_t y_start, struct shapeObj * rect);
static void draw_circle(uint16_t x_center, uint16_t y_center, struct shapeObj * circle);
static void draw_string_scale(int x, int y, char *s, unsigned int len, uint32_t color, int scale);
static void draw_char_scale(int x, int y, char c, uint32_t color, int scale);

struct shapeObj * create_circle(uint16_t r, bool hasFill, uint32_t fill, bool hasOutline, uint32_t outline)
{
	struct shapeObj * circle = malloc(sizeof(struct shapeObj));
	circle->type = SHAPE_CIR
	circle->dim1 = r;
	circle->dim2 = r;
	circle->hasFill = hasFill || !hasOutline;
	circle->fillColor = fill;
	circle->hasOutline = hasOutline;
	circle->outlineColor = outline;
	circle->lineWidth = 2;
	return circle;
}

struct shapeObj * create_rect(uint16_t l, uint16_t h, bool hasFill, uint32_t fill, bool hasOutline, uint32_t outline)
{
	struct shapeObj * rect = malloc(sizeof(struct shapeObj));
	rect->type = SHAPE_RECT
	rect->dim1 = l;
	rect->dim2 = h;
	rect->hasFill = hasFill || !hasOutline;
	rect->fillColor = fill;
	rect->hasOutline = hasOutline;
	rect->outlineColor = outline;
	rect->lineWidth = 2;
	return rect;
}

void draw_shape(uint16_t x, uint16_t y, struct shapeObj * shape, uint8_t justify_h, uint8_t jusitfy_v)
{
	if (shape->type == SHAPE_CIR)
	{
		if (justify_h == JUSTIFY_L)
		{
			x = x + shape->dim1;
		}
		else if (justify_h == JUSTIFY_R)
		{
			x = x - shape->dim1;
		}

		if (justify_v == JUSTIFY_T)
		{
			y = y + shape->dim1;
		}
		else if (justify_v == JUSTIFY_B)
		{
			y = y - shape->dim1;
		}

		draw_circle(x, y, shape);
	}
	if (shape->type == SHAPE_RECT)
	{
		if (justify_h == JUSTIFY_C)
		{
			x = x - (shape->dim1 / 2);
		}
		else if (justify_h == JUSTIFY_R)
		{
			x = x - shape->dim1;
		}

		if (justify_v == JUSTIFY_C)
		{
			y = y - (shape->dim2 / 2);
		}
		else if (justify_v == JUSTIFY_B)
		{
			y = y - shape->dim2;
		}

		draw_rect(x, y, shape);
	}
}

static void draw_rect(uint16_t x_start, uint16_t y_start, struct shapeObj * rect)
{
	uint16_t x_end = x + rect->dim1;
	uint16_t y_end = y + rect->dim2;

	for (int x = x_start; x < x_end; x++) {
        for (int y = y_start; y < y_end; y++) {
        	if ((x < x_start + rect->lineWidth) || (x >= x_end - rect->lineWidth) || (y < y_start + rect->lineWidth) || (y >= y_end - rect->lineWidth))
        	{
        		if (rect->hasOutline)
        		{
        			draw_pixel(x, y, rect->outlineColor);
        		}
        		else
        		{
        			draw_pixel(x, y, rect->fillColor);
        		}
        	}
        	else if (rect->hasFill)
        	{
        		draw_pixel(x, y, rect->fillColor);
        	}
        }
    }
}

static void draw_circle(uint16_t x_center, uint16_t y_center, struct shapeObj * circle)
{
	for (int x = x_center - circle->dim1; x <= x_center + circle->dim1; x++) {
        for (int y = y_center - circle->dim1; y<= y_center + circle->dim1; y++) {
            int distance = sqrt(pow((x-x_center),2) + pow((y-y_center),2));
            if (distance < circle->dim1) {
            	if (distance >= (circle->dim1 - circle->lineWidth))
            	{
            		if (circle->hasOutline)
            		{
            			draw_pixel(x, y, circle->outlineColor);
            		}
            		else
            		{
            			draw_pixel(x, y, circle->fillColor);
            		}
            	}
            	else if (circle->hasFill)
            	{
            		draw_pixel(x, y, circle->fillColor);
            	}
            }
        }
    }
}

void draw_text(int xPos, int yPos, char* str, uint32_t color, int scale)
{
	draw_string_scale(xPos, yPos, str, strlen(str), color, scale);
}

static void draw_string_scale(int x, int y, char *s, unsigned int len, uint32_t color, int scale)
{
	int i, topLeftX, topLeftY;

	topLeftX = x - (4 * len * scale);
	topLeftY = y - (4 * scale);

	if (topLeftX < 0)
	{
		topLeftX = 0;
	}
	if (topLeftY < 0)
	{
		topLeftY = 0;
	}


	// for each char in string
	for (i = 0; i < len; i++) {

		// draw it (x + 8 * i * scale term is necessary since each char is 8 pixels wide, 
		// so rather than moving over 1 pixel for next char, move over 8 * scale pixels)
		draw_char_scale((topLeftX + 8 * i * scale), topLeftY, s[i], color, scale);
	}
}

static void draw_char_scale(int x, int y, char c, uint32_t color, int scale)
{
	int i, j, bits, pixelX, pixelY; //, pixelLocation;

	// each char is 8 pixels tall, so multiply by scale for new character height
	for (i = 0; i < 8 * scale; i++) 
	{
		// gets each horizontal "line" of 8 pixels from the 8x8 pixel grid that makes up each character
		bits = fontdata_8x8[8 * c + (i / scale)];

		// each char is 8 pixels wide (by default)
		// multiply by scale for new char width
		for (j = 0; j < 8 * scale; j++) 
		{
			// pixelX takes into account desired string position, char width, and bpp to determine x coord of current pixel
			// no need to multiply x-value by a constant representing bits per pixel since each write is 4 bytes
			//pixelX = (x + j + var_screeninfo.xoffset);
			pixelX = (x + j);

			// pixelY takes into account desired string position and char height to determine y coord of current pixel in 2-D screen plane
			//pixelY = (y + i + var_screeninfo.yoffset);
			pixelY = (y + i);

			// pixelLocation is location in framebuffer of the current pixel, will need in order to verify validity of location
			//pixelLocation = pixelX + (pixelY * fix_screeninfo.line_length);

			// first check is to make sure absolute location isn't negative (make sure it's within the lower bound of framebuffer memory)
			// second check makes sure the location is within the upper bound of the framebuffer memory
			// third check (bitshift) determines if pixel is a 1 in font chart (i.e., is the current pixel part of the char drawing)
			//if (pixelLocation >= 0 && pixelLocation < fix_screeninfo.smem_len && ((bits >> (7 - (j / scale))) & 1) && var_screeninfo.bits_per_pixel == 32) 
			if ((bits >> (7 - (j / scale))) & 1)
			{	
				// if everything checks out, draw the current pixel onto the screen
				draw_pixel(pixelX, pixelY, color);
			}
			// simple check incase we run into situation where we have a different than expected number of bits per pixel
			// else if (var_screeninfo.bits_per_pixel != 32)
			// {
			// 	printf("Invalid bits per pixel, should be 32, was %d\n", var_screeninfo.bits_per_pixel);
			// }
		}
	}
}
