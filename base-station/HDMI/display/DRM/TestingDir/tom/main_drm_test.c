
#include <stdio.h>
#include <stdlib.h>
#include "DRM_user.h"


#define SIXTYFPSMICROSECONDS 166667

int main()
{
    //struct buf_context *myBuf0 = malloc(sizeof(struct buf_context));
    //struct buf_context *myBuf1 = malloc(sizeof(struct buf_context));

    printf("inside main\n");

    int fd, ret, canPageFlip;

    fd = drm_open();
    drmSetMaster(fd);

    drm_init(fd);

    drmGetCap(fd, DRM_CAP_ASYNC_PAGE_FLIP, &canPageFlip);
    printf("Ability to page flip: (0 if false, 1 if true): %d\n", canPageFlip);


    demo();

    changeActiveBuffer();

    demo2();

   

    int i = 0;
    while(1){
        pageFlip();
        usleep(SIXTYFPSMICROSECONDS);
    }

    

    // pageFlip(myBuf1);



    drm_close();
    drmDropMaster(fd);

    drm_unmap(bufs[0]);
    drm_unmap(bufs[1]);
}

