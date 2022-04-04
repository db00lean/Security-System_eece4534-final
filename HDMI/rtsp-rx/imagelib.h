#include <stdint.h>

enum img_enc {
  // the following assume one byte per channel, like gstreamer.
  IMGENC_RGB,
  IMGENC_ARGB,
};

struct image {
  enum img_enc enc;
  uint64_t time; // unused
  int width;
  int height;
  char *buf;
};

int bytes_per_pixel(enum img_enc enc);

int write_ppm(char *filepath, struct image *img);

struct image *read_ppm(char *filepath);

struct image *create_image(enum img_enc enc, int width, int height);

void free_image(struct image *img);
