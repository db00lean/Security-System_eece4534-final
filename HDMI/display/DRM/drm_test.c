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
    printf("count_fbs: %d \n ", (int)res->count_fbs);
    printf("count_connectors: %d \n ", (int)res->count_connectors);
    printf("count_encoders: %d \n ", (int)res->count_encoders);

    printf("min_width: %d \n ", (int)res->min_width);
    printf("max_width: %d \n ", (int)res->max_width);
    
    printf("min_height: %d \n ", (int)res->min_height);
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

 struct drm_mode_create_dumb create_request = {
		
        .width  = mode->hdisplay,
		.bpp    = 32
	};



    ret = ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create_request);   

    if(ret){
        printf("Failed to make dumb buffer");
        return 0;

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