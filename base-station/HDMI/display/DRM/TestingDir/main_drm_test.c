
#include <stdio.h>

#include "./DRM_user_test.h"


int main()
{
    struct buf_context *myBuf0 = malloc(sizeof(struct buf_context));
    
    printf("inside main\n");

    int fd, ret;
    fd = drm_open();
    drm_init(fd);
    myBuf0->map = drm_map(fd, myBuf0);

    print_info();

    demo(myBuf0);
    getchar();
}
