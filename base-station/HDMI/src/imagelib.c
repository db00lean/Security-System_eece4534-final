// Connor Northway
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/imagelib.h"

int bytes_per_pixel(enum img_enc enc) {
  switch (enc) {
  case IMGENC_RGB:
    return 3;
    break;
  case IMGENC_ARGB:
    return 4;
    break;
  default:
    return -1;
  }
}

// returns non-zero on failure
int write_ppm(char *filepath, struct image *img) {
  if (img->enc != IMGENC_RGB) {
    // this code was naively written for RGB in a buffer
    // can't handle anything else yet
    // (would need to pixel-by-pixel convert)
    return -1;
  }
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

struct image *read_ppm(char *filepath) {
  // read a ppm from a file

  // unimplemented!
  // possibly good for testing.
  return NULL; // hope you check your return values
}

struct image *create_image(enum img_enc enc, int width, int height) {
  int bpp = bytes_per_pixel(enc); // naming?
  if (bpp == -1) {
    return NULL;
  }
  size_t bytes_in_data = width * height * bpp;
  char *buf = malloc(bytes_in_data);
  struct image *img = malloc(sizeof(struct image));

  *img = (struct image){
      .enc = enc, .time = 0, .width = width, .height = height, .buf = buf};

  return img;
}

struct image *create_image_size(enum img_enc enc, size_t size) {
  char *buf = malloc(size);
  struct image *img = malloc(sizeof(struct image));
  *img = (struct image){
      .enc = enc, .time = 0, .width = 0, .height = 0, .buf = buf};
  return img;
}

void free_image(struct image *img) {
  free(img->buf);
  free(img);
}
