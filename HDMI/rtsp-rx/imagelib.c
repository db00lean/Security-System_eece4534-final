#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "imagelib.h"

// returns non-zero on failure
int write_ppm(char *filepath, struct image *img) {
  int rv;
  // write an image struct out to a file as ppm
  FILE *fp = fopen(filepath, "w");
  if (!fp) {
    return -1;
  }
  rv = fprintf(fp, "P6\n%i %i\n255\n", img->width, img->height);
  if (rv < 0) {
    fclose(fp);
    return -1;
  }
  size_t num_bytes = img->height * img->width * 3;
  fwrite(img->buf, num_bytes, 1, fp);
  if (rv != num_bytes) {
    fclose(fp);
    return -1;
  }
  fclose(fp);
  return 0;
}

struct image * read_ppm(char *filepath) {
  // read a ppm from a file

  // unimplemented!
  // probably good for testing.
  exit(1);
}

struct image * create_image(int width, int height) {
  char *buf = malloc(width*height*3);
  struct image *img = malloc(sizeof(struct image));

  img->width = width;
  img->height = height;
  img->buf = buf;

  return img;
}


void free_image(struct image *img) {
  free(img->buf);
  free(img);
}
