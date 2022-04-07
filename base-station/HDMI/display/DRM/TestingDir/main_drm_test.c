#include "DRM_user_test.h"

int main(){

    int fd, ret;
    fd = drm_open();
    drm_init(fd);
    map = drm_map(fd);

    print_info();

    demo();
    getchar();
}
