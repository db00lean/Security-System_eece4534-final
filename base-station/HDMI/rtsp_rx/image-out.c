#include "imagelib.h"

#define IMG_W 1920
#define IMG_H 1080

// indexing:
//  row: y*IMG_W*3
//  col: x*3
#define PIXEL(x, y) ((y * IMG_W * 3) + (x * 3))

int main(int argc, char **argv) {
  struct image *img = create_image(IMGENC_RGB, IMG_W, IMG_H);

  // _very_ quick and dirty example
  // don't replicate this
  // it's just to get an image.
  for (int ii = 0; ii < IMG_H; ii++) {   // rows
    for (int jj = 0; jj < IMG_W; jj++) { // columns
      //  color within pixel: +0,1,2
      img->buf[PIXEL(jj, ii) + 0] = 255 * jj / 3 / IMG_W;
      img->buf[PIXEL(jj, ii) + 1] = 0;
      img->buf[PIXEL(jj, ii) + 2] = 255 * ii / IMG_H;
    }
  }

  // TODO(cnorthway): take as argument?
  write_ppm("./test-image.ppm", img);

  free_image(img);

  return 0;
}
