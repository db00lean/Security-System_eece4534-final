#include <unistd.h>
#include <stdio.h>

#include "gstreamer-rx.h"
#include "imagelib.h"

int main(int argc, char *argv[]) {
  struct camera_rx *cam = init_rx_camera("rtsp.stream/pattern");


  // note to developer: break at these points to check data
  struct image * img = get_frame(cam, IMGENC_BGR, 640, 480);
  printf("first\n");
  free_image(img);
  sleep(1);
  img = get_frame(cam, IMGENC_BGR, 640, 480);
  printf("second\n");
  free_image(img);
  sleep(1);
  img = get_frame(cam, IMGENC_BGR, 640, 480);
  printf("third\n");
  free_image(img);
  sleep(1);
  img = get_frame(cam, IMGENC_BGR, 640, 480);
  printf("fourth\n");
  free_image(img);

  cleanup_rx_camera(cam);

  printf("done\n");
  return 0;
}
