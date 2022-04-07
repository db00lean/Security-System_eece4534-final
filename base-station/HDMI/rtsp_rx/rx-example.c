#include <unistd.h>
#include <stdio.h>

#include "gstreamer-rx.h"
#include "imagelib.h"

int main(int argc, char *argv[]) {

  struct camera_rx *cam = init_rx_camera("rtsp.stream/pattern");


  // note to developer: break at these points to check data
  struct image * img = get_frame(cam, IMGENC_ARGB);
  free_image(img);
  sleep(1);
  img = get_frame(cam, IMGENC_ARGB);
  free_image(img);
  sleep(1);
  img = get_frame(cam, IMGENC_ARGB);
  free_image(img);
  sleep(1);
  img = get_frame(cam, IMGENC_ARGB);
  free_image(img);

  printf("done\n");
  return 0;
}
