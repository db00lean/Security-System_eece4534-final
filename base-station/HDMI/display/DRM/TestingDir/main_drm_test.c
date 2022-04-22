#include "./DRM_user_test.h"

int main(){

    int fd, ret;
    struct buf_context bufs[2];
    fd = drm_open();
    drm_init(fd);
    makeFB(fd, &bufs[0]);

    //makeFB(fd, &bufs[1]);
    //map = drm_map(fd);

    print_info();
    printf("before demo 1\n");
    demo(bufs[0]);
    getchar();
    printf("after demo 1\n\n");


    /*
    // getchar();
    // pageFlip(fd, &bufs[1]);
    // printf("before demo 2\n");
    // demo2(bufs[1]);
    // printf("after demo 2\n");
    // getchar();
    // pageFlip(fd, &bufs[0]);
    */
}
