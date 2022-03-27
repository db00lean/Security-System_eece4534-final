#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


#include <xf86drm.h>
#include <xf86drmMode.h>


//This is the default and only card on the zedboard
#define cardPath "/dev/dri/card0"


drmModeRes *res;
drmModeConnector *conn;
drmModeModeInfo *mode;
drmModeEncoder *encode;


int drm_open();
int drm_init(int fd);
int drm_close();

int main(){
    
    void *map;
    int ret;
    int fd = drm_open();
    
  

    if(fd == -1){
        printf("fd failed to open");
        return -1;
    }

    if(drm_init(fd) == -1){
        printf("Error with init DRM");
        return -1;
    }

  
    printf("######## RESOURCE ########\n");
    printf("count_fbs: %d \n", (int)res->count_fbs);
    printf("count_connectors: %d \n", (int)res->count_connectors);
    printf("count_encoders: %d \n", (int)res->count_encoders);

    printf("min_width: %d \n", (int)res->min_width);
    printf("max_width: %d \n", (int)res->max_width);
    
    printf("min_height: %d \n", (int)res->min_height);
    printf("max_height: %d \n ", (int)res->max_height);

  
      

    printf("######## CONNECTOR ########\n");
    printf("connector %d\n", conn->connector_id);

    printf("mmWidth: %d\n", conn->mmWidth);
    printf("mmHeight: %d\n", conn->mmHeight);
    printf("encoder_id: %d\n", conn->encoder_id);
    printf("connector_type_id %d\n", conn->connector_type);
    

    printf("######## MODE ########\n");
    printf("hdisplay: %d\n", mode->hdisplay);
    printf("vdisplay: %d\n", mode->vdisplay);
    printf("vrefresh: %d\n", mode->vrefresh);



    uint32_t fb;
    struct drm_mode_create_dumb crereq;
    struct drm_mode_destroy_dumb dreq;
    struct drm_mode_map_dumb mreq;

    memset(&crereq, 0, sizeof(crereq));

	crereq.height = mode->vdisplay;
    crereq.width = mode->hdisplay;
	crereq.bpp = 32;



    ret = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &crereq);   


    if(ret ){
        printf("Failed to make dumb buffer");
        return ret;

    }

    ret = drmModeAddFB(fd,  (uint32_t) crereq.width, (uint32_t) crereq.height, 24,
                     crereq.bpp, crereq.pitch, crereq.handle, &fb);

                     
    if(ret){
        printf("Failed to add dumb buffer\n");
        printf("ret: %d\n", ret);
        return ret;
    }

    printf("fb: %d\n", fb);


    memset(&mreq, 0, sizeof(mreq));
    mreq.handle = crereq.handle;
    ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);
   
    if (ret) {
        printf("Failed to set map dumb");
        return -1;
    }
     

    map = mmap(0, crereq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);


    if (map == MAP_FAILED) {
        printf("map failed");
        return -1;
    }


  
   
    memset(map, 0, crereq.size);


    //drmModeSetCrtc(fd, encode->crtc_id, fb, 0,0, &conn->connector_id, 1, mode);

    printf("size %d\n", crereq.size);


    uint32_t const red   = (0xff<<16);
	uint32_t const green = (0xff<<8);
	uint32_t const blue  = (0xff);
	uint32_t const colors[] = {red, green, blue};

   int i;
   for(i = 0; i < 1000000; i++){

       ((uint32_t *) map)[i] = colors[rand()%3];
   }








    drm_close();

    return 0;
}

int drm_init(int fd){
  res = drmModeGetResources(fd);
    
    if(res == NULL){
        printf("Failed to get resources");
        return -1;
    }

  conn = drmModeGetConnector(fd, res->connectors[0]);
    
    if(conn == NULL){
        printf("Can't find connector");
          printf("Can't find connector");
        return -1;
       
    }

    encode = drmModeGetEncoder(fd, res->encoders[0]);

    if(encode->encoder_id != conn->encoder_id){
        printf("error with encoder and connector IDs");
        return -1;
    }


    mode = conn->modes;





    return 0;
}

int drm_close(){


    drmModeFreeConnector(conn);
    drmModeFreeEncoder(encode);
    drmModeFreeResources(res);

    return 0;
}


int drm_open(){
    
    int fd, flags;

    //Opening cards
    fd = open("/dev/dri/card0", O_RDWR);

    if(fd == NULL){
        return -1;
    }

    return fd;
}