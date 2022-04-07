// This file is used to write a string to the framebuffer and display it.
// Additional inputs include location (center coordinates) and color.
// Additional functions to clear an area and clear an area and write a string.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include "../inc/drawtext.h"

//static void *fbMemPtr;

//static struct fb_var_screeninfo var_screeninfo;
//static struct fb_fix_screeninfo fix_screeninfo;

static void draw_pixel(int x, int y, uint32_t color);
static void draw_string(int x, int y, char *s, unsigned int len, uint32_t color);
static void draw_string_scale(int x, int y, char *s, unsigned int len, uint32_t color, int scale);
//static void clear_string(int x, int y, int charsToClear);
static void draw_char(int x, int y, char c, uint32_t color);
static void draw_char_scale(int x, int y, char c, uint32_t color, int scale);
//static void clear_area(int x, int y, int w, int h);

void draw_text(int xPos, int yPos, char* str, uint32_t color)
{
    draw_string(xPos, yPos, str, strlen(str), color);
}

void draw_text_scale(int xPos, int yPos, char* str, uint32_t color, int scale)
{
	draw_string_scale(xPos, yPos, str, strlen(str), color, scale);
}

// void clear_text(int xPos, int yPos, int charsToClear)
// {
//     clear_string(xPos, yPos, charsToClear);
// }

// void clear_and_draw_text(int xPos, int yPos, char* str, uint32_t color)
// {
//     clear_string(xPos, yPos, strlen(str));
//     draw_string(xPos, yPos, str, strlen(str), color);
// }


//only for testing in main
// void fb_open_test(){
//     int fd;
// 	fd = open("/dev/fb0", O_RDWR);

//     if (fd == -1){
//         printf("bad file\n");
//     }
// 	ioctl(fd, FBIOGET_VSCREENINFO, &var_screeninfo);
// 	ioctl(fd, FBIOGET_FSCREENINFO, &fix_screeninfo);
    
//     fbMemPtr = mmap(0,
// 			var_screeninfo.yres_virtual * fix_screeninfo.line_length,
// 			PROT_WRITE | PROT_READ,
// 			MAP_SHARED, fd, 0);
//     // printf("fbmem pointer initial %x\n", fbMemPtr);

// }

// basic draw_pixel implementation like the one done in boundingbox.c
// static void draw_pixel(int x, int y, uint32_t color)
// {
//     uint32_t *pixelPtr;
//     pixelPtr = fbMemPtr;
// 	//line length is is bytes, so divide by 4 so we can treat it as pixels
//     pixelPtr += (fix_screeninfo.line_length / 4) * y;
//     pixelPtr += x;

//     // printf("fbMemPtr: 0x%x\n", fbMemPtr);
//     // printf("pixelPtr: 0x%x\n", pixelPtr);

//     // printf("x %d, y %d, color 0x%x\n", x, y, color);
//     *pixelPtr = color;
// }

static void draw_string(int x, int y, char *s, unsigned int len, uint32_t color)
{
	int i, topLeftX, topLeftY;

	topLeftX = x - (4 * len);
	topLeftY = y - 4;

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

		// draw it (x + 8 * i term is necessary since each char is 8 pixels wide, so rather than moving over 1 pixel for next char, move over 8 pixels)
		draw_char((topLeftX + 8 * i), topLeftY, s[i], color);
	}
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

// static void clear_string(int x, int y, int charsToClear)
// {
//     clear_area(x, y, charsToClear * 8, 8);
// }

static void draw_char(int x, int y, char c, uint32_t color)
{
	int i, j, bits, pixelX, pixelY; //, pixelLocation;

	// each char is 8 pixels tall
	for (i = 0; i < 8; i++) 
	{
		// gets each horizontal "line" of 8 pixels from the 8x8 pixel grid that makes up each character
		bits = fontdata_8x8[8 * c + i];

		// each char is 8 pixels wide (by default)
		for (j = 0; j < 8; j++) 
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
			//pixelLocation = pixelX + (pixelY * fix_screeninfo.line_length);

			// first check is to make sure absolute location isn't negative (make sure it's within the lower bound of framebuffer memory)
			// second check makes sure the location is within the upper bound of the framebuffer memory
			// third check (bitshift) determines if pixel is a 1 in font chart (i.e., is the current pixel part of the char drawing)
			//if (pixelLocation >= 0 && pixelLocation < fix_screeninfo.smem_len && ((bits >> (7 - j)) & 1) && var_screeninfo.bits_per_pixel == 32)
			if ((bits >> (7 - j)) & 1)
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

// static void clear_area(int x, int y, int w, int h)
// {
// 	int i;
// 	int pixelX, pixelY, pixelLocation;
// 	char *fbBuffer = (char *)fbMemPtr;

// 	// to clear a known rectangular space, go to each row in height h and do a blanket write of width w
// 	for (i = 0; i < h; i++) {

// 		// like draw_char, first term determines x location of row to right, second determines y location
// 		// here, fix_screeninfo.line_length is multiplied to get the exact location in the buffer, rather than the y-coord in the screen's 2-D plane
// 		pixelLocation = (x + var_screeninfo.xoffset) * (var_screeninfo.bits_per_pixel / 8)
// 					  + (y + i + var_screeninfo.yoffset) * fix_screeninfo.line_length;

// 		// writes value of 0 (black) to section of framebuffer, figured this was easier than going pixel by pixel
// 		memset(fbBuffer + pixelLocation, 0, w * var_screeninfo.bits_per_pixel / 8);
// 	}
// }

// for testing only
// int main(void)
// {
//     fb_open_test();

//     char testTextString[] = "Bomba";

//     int strX = 100;
//     int strY = 100;

//     // clear arbitrary amount of space at least as large as string to write
//     //clear_text(strX, strY, 5);
//     //draw_text(strX, strY, testTextString, 0xFFFFFFFF);

//     // perform same thing as above with single call but in different location
//     //clear_and_draw_text(strX, strY + 10, testTextString, 0xFFFFFFFF);

// 	draw_text(70, 70, "scale1", 0xFFFFFFFF);
// 	draw_text_scale(100, 100, "scale2", 0xFFFFFFFF, 2);
// 	draw_text_scale(170, 170, "scale3", 0xFFFFFFFF, 3);


//     return 0;
// }