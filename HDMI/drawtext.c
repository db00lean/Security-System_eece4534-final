// This file is used to write a string to the framebuffer and display it.
// Additional inputs include location ("top-left" coordinates) and color.
// Additional functions to clear an area and clear an area and write a string.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <linux/fb.h>

#include "drawtext.h"

static void *fbMemPtr;

static struct fb_var_screeninfo var_screeninfo;
static struct fb_fix_screeninfo fix_screeninfo;


static void fb_put_string(int x, int y, char *s, unsigned int maxlen, unsigned int color);
static void fb_clear_string(int x, int y, int charsToClear);
static void fb_put_char(int x, int y, char c, unsigned int color);
static void fb_clear_area(int x, int y, int w, int h);

void draw_text(int xPos, int yPos, char* str, unsigned int color)
{
    fb_put_string(xPos, yPos, str, strlen(str), color);
}

void clear_text(int xPos, int yPos, int charsToClear)
{
    fb_clear_string(xPos, yPos, charsToClear);
}

void clear_and_draw_text(int xPos, int yPos, char* str, unsigned int color)
{
    fb_clear_string(xPos, yPos, strlen(str));
    fb_put_string(xPos, yPos, str, strlen(str), color);
}


// only for testing in main
void fb_open_test(){
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
    // printf("fbmem pointer initial %x\n", fbMemPtr);

}

static void fb_put_string(int x, int y, char *s, unsigned int maxlen, unsigned int color)
{
	int i;
	int w = 0;

	for (i = 0; i < strlen(s) && i < maxlen; i++) {
		fb_put_char((x + 8 * i), y, s[i], color);
		w += 8;
	}

    // maybe return this for error checking if we want to validate how many pixels wide the entire write was
	// return w;
}

static void fb_clear_string(int x, int y, int charsToClear)
{
    fb_clear_area(x, y, charsToClear * 8, 8);
}

static void fb_put_char(int x, int y, char c, unsigned int color)
{
	int i, j, bits, loc;
	unsigned short *p16;
	unsigned int *p32;

	for (i = 0; i < 8; i++) {
		bits = fontdata_8x8[8 * c + i];
		for (j = 0; j < 8; j++) {
			loc = (x + j + var_screeninfo.xoffset) * (var_screeninfo.bits_per_pixel / 8)
				+ (y + i + var_screeninfo.yoffset) * fix_screeninfo.line_length;
			if (loc >= 0 && loc < fix_screeninfo.smem_len &&
					((bits >> (7 - j)) & 1)) {
				switch (var_screeninfo.bits_per_pixel) {
				case 16:
                    // replace this with a general call to draw_pixel once we decide how to generalize it
					p16 = fbMemPtr + loc;
					*p16 = color;
					break;
				case 24: // not sure why 24bpp is not supported, perhaps more reseach necessary
				case 32:
                    // replace this with a general call to draw_pixel once we decide how to generalize it
					p32 = fbMemPtr + loc;
					*p32 = color;
					break;
				}
			}
		}
	}
}

static void fb_clear_area(int x, int y, int w, int h)
{
	int i = 0;
	int loc;
	char *fbuffer = (char *)fbMemPtr;

	for (i = 0; i < h; i++) {
		loc = (x + var_screeninfo.xoffset) * (var_screeninfo.bits_per_pixel / 8)
			+ (y + i + var_screeninfo.yoffset) * fix_screeninfo.line_length;
		memset(fbuffer + loc, 0, w * var_screeninfo.bits_per_pixel / 8);
	}
}

// for testing only
int main(void)
{
    fb_open_test();

    char testTextString[] = "Bomba";

    int strX = 50;
    int strY = 50;

    // clear arbitrary amount of space at least as large as string to write
    clear_text(strX, strY, 5);
    draw_text(strX, strY, testTextString, 0xFFFFFF);

    // perform same thing as above with single call and change locations slightly
    clear_and_draw_text(strX, strY + 10, testTextString, 0xFFFFFF);

    return 0;
}