
#include <stdio.h>
#include <stdlib.h>
#include "DRM_user_test.h"
#include <unistd.h>

#define SIXTYFPSMICROSECONDS 166667

int main()
{
    //struct buf_context *myBuf0 = malloc(sizeof(struct buf_context));
    //struct buf_context *myBuf1 = malloc(sizeof(struct buf_context));

    printf("inside main\n");

    int fd, ret;

    fd = drm_open();
    printf("Opened device");
    drmSetMaster(fd);

    drm_init(fd);


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

   // drm_unmap(bufs[0]);
    //drm_unmap(bufs[1]);
}

