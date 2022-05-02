#include <unistd.h>
#include <stdio.h>

#include "gstreamer-rx.h"
#include "imagelib.h"

int main(int argc, char *argv[]) {
  struct camera_rx *cam = init_rx_camera("rtsp.stream/pattern");


  // note to developer: break at these points to check data
  for (int ii = 0; ii < 5; ii++) {
    struct image * img = get_frame(cam, IMGENC_ARGB, 320, 180);
    free_image(img);
    printf("number %d\n", ii);
  }

  cleanup_rx_camera(cam);

  printf("done\n");
  return 0;
}
