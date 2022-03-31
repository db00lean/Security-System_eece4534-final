#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


#include <xf86drm.h>
#include <xf86drmMode.h>


#include <drm_fourcc.h>
//BGRA8888 -- alpha (transparency r g b, 8 bits for each)
//drmModeAddFB2(drm_fd, width, height, DRM_FORMAT_XRGB8888,
//    handles, strides, offsets, &fb_id, 0);

//This is the default and only card on the zedboard
#define cardPath "/dev/dri/card0"


//
drmModeRes *res;
drmModeConnector *conn;
drmModeModeInfo *mode;
drmModeEncoder *encode;
drmModeCrtc *crtc;

drmModeFB *fb;

void *map;



int drm_open();
int drm_init(int fd);
int drm_close();
void draw_pixel(int x, int y, uint32_t ARGB);
void demo(int resolution);


uint32_t const red   = (0xff<<16);
uint32_t const green = (0xff<<8);
uint32_t const blue  = (0xff);
uint32_t const colors[] = {red, green, blue};

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
    printf("count_fbs: %d \n", (int)res->count_fbs);
    printf("count_connectors: %d \n", (int)res->count_connectors);
    printf("count_encoders: %d \n", (int)res->count_encoders);

    printf("min_width: %d \n", (int)res->min_width);
    printf("max_width: %d \n", (int)res->max_width);
    
    printf("min_height: %d \n", (int)res->min_height);
    printf("max_height: %d \n ", (int)res->max_height);

    printf("connector width %d height %d\n", conn->mmWidth, conn->mmHeight);

  
      

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



    //32 bit memory location to store address of framebuffer
    uint32_t fb;
    
    //struct to create dumb buffer
    struct drm_mode_create_dumb crereq;

    //struct to create memory mapping for dumb buffer
    struct drm_mode_map_dumb mreq;

    //struct to destroy dumb buffer
    struct drm_mode_destroy_dumb dreq;

    //clear crereq before setting members
    memset(&crereq, 0, sizeof(crereq));

    //set members of crereq based on members of "drmModeModeInfo" obtained in drm_init()
	crereq.height = mode->vdisplay;
    crereq.width = mode->hdisplay;
	crereq.bpp = 32;


    //create dumb DRM based on crereq members -- "handle, pitch, size will be returned", members of crereq
    ret = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &crereq);   


    if(ret ){
        printf("Failed to make dumb buffer");
        return ret;

    }
    //create DRM FB using information within crereq, populated by drmIoctl call above
    ret = drmModeAddFB(fd,  (uint32_t) crereq.width, (uint32_t) crereq.height, 24,
                     crereq.bpp, crereq.pitch, crereq.handle, &fb);

                     
    if(ret){
        printf("Failed to create DRM buffer\n");
        printf("ret: %d\n", ret);
        return ret;
    }

    printf("fb: %d\n", fb);
    //wont work because fb is uint32_t in scope, doesn't refer to struct
    //printf("bits per pixel %d\n",(int)fb->bpp); 

    //Clear mreq
    memset(&mreq, 0, sizeof(mreq));

    //Set memory mapping handle equal to the handle of the dumb fb just created
    mreq.handle = crereq.handle;
    //Map dumb buffer based on mreq.handle
    //This Ioctl call populates mreq.offset, used in mmap call below
    ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);
   
    if (ret) {
        printf("Failed to set map dumb");
        return -1;
    }
     
    //Map memory region for DRM framebuffer using size and mapped offset of dumbbuffer
    map = mmap(0, crereq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);


    if (map == MAP_FAILED) {
        printf("map failed");
        return -1;
    }


  
   
    memset(map, 0, crereq.size);


    



    // uint32_t const red   = (0xff<<16);
	// uint32_t const green = (0xff<<8);
	// uint32_t const blue  = (0xff);
	// uint32_t const colors[] = {red, green, blue};

   int i;
   int count = 0;
   //float resolution = mode->vdisplay * mode->hdisplay;
   int resolution = mode->vdisplay * mode->hdisplay;
    printf("size %d\n", crereq.size);
    printf("resolution %d\n", resolution);
    printf("calculated resolution %d\n", mode->vdisplay * mode->hdisplay);
    printf("test\n");
    //printf("calculated bits per pixel %d\n", crereq.size / resolution);

//    for(i = 0; i < (int)resolution; i++){


//        if(count < (resolution /3)){
//         ((uint32_t *) map)[i] = colors[0];
//        }

//        else if( count <  ((resolution * 2.0/3.0))){
//        ((uint32_t *) map)[i] = colors[1];
//        }
//        else{
//            ((uint32_t *) map)[i] = colors[2];
//        }

//        count = count + 1;
//    }
    demo(resolution);


    
    drmSetMaster(fd);

    //clear crtc
    drmModeSetCrtc(fd, crtc->crtc_id, 0, 0,0, NULL, 0, NULL);

    drmModeSetCrtc(fd, crtc->crtc_id, fb, 0,0, &conn->connector_id, 1, mode);

    drmDropMaster(fd);

    drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);

    drmModePlaneRes *planes = drmModeGetPlaneResources(fd);
    printf("num planes %d\n", planes->count_planes);

    // for (uint32_t i = 0; i < planes->count_planes; i++) {
    //     uint32_t plane_id = planes->planes[i];
    //     drmModePlane *plane = drmModeGetPlane(drm_fd, plane_id);
    // }


    getchar();

    drm_close();

    return 0;
}

int drm_init(int fd){
    //point our "struct drmModeRes" -- contains information about current display configuration
  res = drmModeGetResources(fd);
    
    if(res == NULL){
        printf("Failed to get resources");
        return -1;
    }
    //point our "struct drmModeConnector" based on "connector" member defined within "struct drmModeRes" above
  conn = drmModeGetConnector(fd, res->connectors[0]);
    
    if(conn == NULL){
        printf("Can't find connector");
          printf("Can't find connector");
        return -1;
       
    }
    //point our "struct drmModeEncoder" encoder information based on "encoder" member defined within "struct drmModeRes" above
    encode = drmModeGetEncoder(fd, res->encoders[0]);

    if(encode->encoder_id != conn->encoder_id){
        printf("error with encoder and connector IDs");
        return -1;
    }
    //point our "struct drmModeCrtc" information based on "CRTC_id" member defined within "struct drmModeEncoder" above
    crtc = drmModeGetCrtc(fd, encode->crtc_id);

    //set "struct drmModeModeInfo" to the "struct drmModeModeInfo" member contained within "drmModeConnector"
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
void draw_pixel(int x, int y, uint32_t ARGB){
    uint32_t *pixelPtr;
    pixelPtr = map;
    pixelPtr += mode->hdisplay * y;
    pixelPtr += x;

    *pixelPtr = ARGB;
}
void demo(int resolution){
    int y, x;
    for(y = 0; y < mode->vdisplay; y++){
        for (x  = 0; x < mode->hdisplay; x++) {
            if ( y < (mode->vdisplay/3)) {
                draw_pixel(x, y, colors[0]);
            }
            else if (y < (mode->vdisplay * 2/3)) {
                draw_pixel(x, y, colors[1]);
            }
            else {
                draw_pixel(x, y, colors[2]);
            }
        }
    }
}

    // for(i = 0; i < (int)resolution; i++){
    //    if(i < (resolution /3)){
    //     ((uint32_t *) map)[i] = colors[0];
    //    }

    //    else if( i <  ((resolution * 2.0/3.0))){
    //    ((uint32_t *) map)[i] = colors[1];
    //    }
    //    else{
    //        ((uint32_t *) map)[i] = colors[2];
    //    }
  // }
//}
