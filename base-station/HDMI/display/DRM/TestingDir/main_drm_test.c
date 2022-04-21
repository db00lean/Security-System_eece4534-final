
#include <stdio.h>
#include <stdlib.h>
#include "./DRM_user_test.h"

#define BUFF_AMOUNTS 2

int main()
{
    //struct buf_context *myBuf0 = malloc(sizeof(struct buf_context));
    //struct buf_context *myBuf1 = malloc(sizeof(struct buf_context));

    printf("inside main\n");

    int fd, ret;
    struct buf_context *bufs[BUFF_AMOUNTS];

    int i = 0;
    for (i = 0; i < BUFF_AMOUNTS; i++){
        bufs[i] = malloc(sizeof(struct buf_context));
    }

    fd = drm_open();
    drmSetMaster(fd);

    bufs[0]->fd=fd;
    bufs[1]->fd=fd;

    drm_init(fd);
    bufs[0]->map = drm_map(bufs[0]->fd, bufs[0], 0);
    //print_info();
    demo(bufs[0]);

    printf("\n\nwaiting for char on framebuffer %d\n\n", bufs[0]->fb);


    bufs[1]->map = drm_map( bufs[1]->fd,  bufs[1], 1);
    demo2( bufs[1]);
    printf("\n\nwaiting for char on framebuffer %d\n\n", bufs[1]->fb);
    
   

    i =0;
    while(1){
        if(i == 0){
             pageFlip(fd,  bufs[0]);
             i = 1;
        }
        else{
             pageFlip(fd,  bufs[1]);
             i = 0;
        }
     usleep(1000000/2);

    }

    

    // pageFlip(myBuf1);



    drm_close();
    drmDropMaster(fd);

    drm_unmap(bufs[0]);
    drm_unmap(bufs[1]);
}
