#include "./DRM_user_test.h"

int main(){

    int fd, ret;
    struct buf_context bufs[2];
    fd = drm_open();
    drm_init(fd);
    makeFB(fd, &bufs[0]);

    makeFB(fd, &bufs[1]);
    //map = drm_map(fd);

    print_info();

    demo(bufs[0]);
   // getchar();
   pageFlip(fd, &bufs[0]);
}
