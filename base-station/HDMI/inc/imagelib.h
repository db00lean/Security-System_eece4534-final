#ifndef __IMAGELIB_H__
#define __IMAGELIB_H__

#include <stdint.h>
#include <stddef.h>

enum img_enc {
  // the following assume one byte per channel, like gstreamer.
  IMGENC_RGB,
  IMGENC_ARGB,
  IMGENC_BGR,
};

struct image {
  enum img_enc enc;
  uint64_t time; // unused
  int width;
  int height;
  size_t buf_len;
  char *buf;
};

int bytes_per_pixel(enum img_enc enc);

int write_ppm(char *filepath, struct image *img);

struct image *read_ppm(char *filepath);

struct image *create_image(enum img_enc enc, int width, int height);

// better for our uses
struct image *create_image_size(enum img_enc enc, size_t size);

void free_image(struct image *img);

#endif
