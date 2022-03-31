/*
I am including this because it is not on the branch I am working on, but this is in the cnorthway/hdmi-gstreamer branch
under project-security-system/HDMI/rtsp-rx/imagelib.h
*/

#include <stdint.h>

#ifndef __IMAGELIB__
#define __IMAGELIB__

// not quite a PPM but lol, it totally is
struct image {
  int width;
  int height;
  // uint16_t depth; // assuming max 255 per channel
  // if we really want more than 8 bits of depth we can modify this
  char *buf;
};


int write_ppm(char *filepath, struct image *img);

struct image * read_ppm(char *filepath);

struct image * create_image(int width, int height);

void free_image(struct image *img);

#endif