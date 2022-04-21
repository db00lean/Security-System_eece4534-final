
#include <stdio.h>
#include <stdlib.h>
#include "./DRM_user_test.h"



int main()
{
    //struct buf_context *myBuf0 = malloc(sizeof(struct buf_context));
    //struct buf_context *myBuf1 = malloc(sizeof(struct buf_context));

    printf("inside main\n");

    int fd, ret;

    struct buf_context *bufs[BUFF_AMOUNTS];

    fd = drm_open();
    drmSetMaster(fd);

    int i = 0;
    for (i = 0; i < BUFF_AMOUNTS; i++) {
        bufs[i] = malloc(sizeof(struct buf_context));


        bufs[i]->fd = fd;


        drm_init(fd);
        bufs[i]->map = drm_map(bufs[i]->fd, bufs[i], i);
        //print_info();

    }


    demo(bufs[0]);





    demo2( bufs[1]);

   

    i =0;
    while(1){
        if(i == 0){
            demo(bufs[0]);
             pageFlip(fd,  bufs[0]);
             i = 1;
        }
        else{
            demo2( bufs[1]);
             pageFlip(fd,  bufs[1]);
             i = 0;
        }
     usleep(SIXTYFPSMICROSECONDS);

    }

    

    // pageFlip(myBuf1);



    drm_close();
    drmDropMaster(fd);

    drm_unmap(bufs[0]);
    drm_unmap(bufs[1]);
}
