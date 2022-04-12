
#include <stdio.h>
#include <stdlib.h>
#include "./DRM_user_test.h"


int main()
{
    struct buf_context *myBuf0 = malloc(sizeof(struct buf_context));
    struct buf_context *myBuf1 = malloc(sizeof(struct buf_context));

    printf("inside main\n");

    int fd, ret;
    fd = drm_open();
    drm_init(fd);
    myBuf0->map = drm_map(fd, myBuf0);
    print_info();
    demo(myBuf0);
    printf("\n\nwaiting for char\n\n");
    getchar();
    getchar();

    drm_unmap(myBuf0);

    myBuf1->map = drm_map(fd, myBuf1);
    demo2(myBuf1);
    printf("\n\nwaiting for char\n\n");
    getchar();
    getchar();
    
}
