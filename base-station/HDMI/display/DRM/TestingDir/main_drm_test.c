
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
    drmSetMaster(fd);

    myBuf0->fd=fd;
    myBuf1->fd=fd;

    drm_init(fd);
    myBuf0->map = drm_map(myBuf0->fd, myBuf0, 0);
    print_info();
    demo(myBuf0);

    printf("\n\nwaiting for char on framebuffer %d\n\n", myBuf0->fb);
    

    myBuf1->map = drm_map(myBuf1->fd, myBuf1, 1);
    demo2(myBuf1);
    printf("\n\nwaiting for char on framebuffer %d\n\n", myBuf1->fb);
    
   

    int i =0;
    while(1){
        if(i == 0){
             pageFlip(fd, myBuf0);
             i = 1;
        }
        else{
             pageFlip(fd, myBuf1);
             i = 0;
        }
     usleep(1000000/2);

    }

    
    // pageFlip(myBuf1);



    drm_close();
    drmDropMaster(fd);

    drm_unmap(myBuf0);
    drm_unmap(myBuf1);

}
