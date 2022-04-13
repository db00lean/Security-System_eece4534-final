
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

    myBuf0->fd=fd;
    myBuf1->fd=fd;

    drm_init(fd);
    myBuf0->map = drm_map(myBuf0->fd, myBuf0);
    print_info();
    demo(myBuf0);

    printf("\n\nwaiting for char on framebuffer %d\n\n", myBuf0->fb);
    getchar();
    getchar();

    myBuf1->map = drm_map(myBuf1->fd, myBuf1);
    demo2(myBuf1);
    printf("\n\nwaiting for char on framebuffer %d\n\n", myBuf1->fb);
    getchar();
    getchar();

    pageFlip(myBuf0);
    usleep(1000000);
    pageFlip(myBuf1);



    drm_unmap(myBuf0);

    
    
}
