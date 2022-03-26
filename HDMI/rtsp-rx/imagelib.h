#include <stdint.h>

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
