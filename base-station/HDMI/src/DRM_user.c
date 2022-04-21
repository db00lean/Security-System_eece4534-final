#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


#include <drm_fourcc.h>

#include "DRM_user.h"


#define PIXEL(x, y) ((y * IMG_W * 3) + (x * 3))


// This is the default and only card on the zedboard
#define cardPath "/dev/dri/card0"

//Structs from libdrm that contain information about DRM objects
drmModeRes *res;
drmModeConnector *conn;
drmModeModeInfo *mode;
drmModeEncoder *encode;
drmModeCrtc *crtc;
drmModeFB *fb;
int CRTC_FB;
//Pointer to memory mapped region for writing to card
//moved to struct
//void *map;





//Defining constants for colors according to default pixel format -- 32 bit word with transparency, red, green, and blue values
uint32_t const red = (0xff << 16);
uint32_t const green = (0xff << 8);
uint32_t const blue = (0xff);
uint32_t const colors[] = {red, green, blue};

int drm_open()
{

    int fd;

    // Opening cards
    fd = open("/dev/dri/card0", O_RDWR);

    if (fd == -1)
    {
        return -1;
    }

    return fd;
}
int drm_init(int fd)
{
    // point our "struct drmModeRes" -- contains information about current display configuration
    res = drmModeGetResources(fd);

    if (res == NULL)
    {
        printf("Failed to get resources");
        return -1;
    }
    // point our "struct drmModeConnector" based on "connector" member defined within "struct drmModeRes" above
    conn = drmModeGetConnector(fd, res->connectors[0]);

    if (conn == NULL)
    {
        printf("Can't find connector");
        printf("Can't find connector");
        return -1;
    }
    // point our "struct drmModeEncoder" encoder information based on "encoder" member defined within "struct drmModeRes" above
    encode = drmModeGetEncoder(fd, res->encoders[0]);

    if (encode->encoder_id != conn->encoder_id)
    {
        printf("error with encoder and connector IDs");
        return -1;
    }
    // point our "struct drmModeCrtc" information based on "CRTC_id" member defined within "struct drmModeEncoder" above
    crtc = drmModeGetCrtc(fd, encode->crtc_id);
    crtc = drmModeGetCrtc(fd, encode->crtc_id);

    // set "struct drmModeModeInfo" to the "struct drmModeModeInfo" member contained within "drmModeConnector"
    mode = conn->modes;



    CRTC_FB = crtc->crtc_id;
    current_buff = 0;
    //Inits frame buffers
    int i = 0;
    for (i = 0; i < BUFF_AMOUNTS; i++) {

        //printf("init frame buffers\n");
        bufs[i] = malloc(sizeof(struct buf_context));


        bufs[i]->fd = fd;



        bufs[i]->map = drm_map(bufs[i]->fd, bufs[i], i);
        //print_info();

    }

    return 0;
}
void *drm_map(int fd, struct buf_context *myBuf, int id)
{
    // 32 bit memory location to store address of framebuffer
    //printf("inside drm map\n");
    uint32_t *fb = malloc(sizeof(uint32_t));
    //printf("fb id %d\n", *fb);
    fb = &myBuf->fb;
    //printf("fb id %d\n", *fb);

    //printf("\n\n MAP getting crtc \n\n");
    crtc = drmModeGetCrtc(fd, encode->crtc_id);
    //printf("crtc id %d\n", crtc->crtc_id);


    int ret;


    // clear crereq before setting members
    memset(&myBuf->crereq, 0, sizeof(myBuf->crereq));

    // set members of crereq based on members of "drmModeModeInfo" obtained in drm_init()
    myBuf->crereq.height = mode->vdisplay;
    myBuf->crereq.width = mode->hdisplay;
    myBuf->crereq.bpp = 32;
    //printf("before drm ioctl\n");
    // create dumb DRM based on crereq members -- "handle, pitch, size will be returned", members of crereq
    ret = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &myBuf->crereq);

    if (ret)
    {
        printf("Failed to make dumb buffer");
        return MAP_FAILED;
    }
    // create DRM FB using information within crereq, populated by drmIoctl call above
    //printf("before add fb\n");
    ret = drmModeAddFB(fd, (uint32_t)myBuf->crereq.width, (uint32_t)myBuf->crereq.height, 24,
                       myBuf->crereq.bpp, myBuf->crereq.pitch, myBuf->crereq.handle, fb);

    if (ret)
    {
        printf("Failed to create DRM buffer\n");
        printf("ret: %d\n", ret);
        return MAP_FAILED;
    }




    // Clear mreq
    memset(&myBuf->mreq, 0, sizeof(myBuf->mreq));

    // Set memory mapping handle equal to the handle of the dumb fb just created
    myBuf->mreq.handle = myBuf->crereq.handle;
    // Map dumb buffer based on mreq.handle
    // This Ioctl call populates mreq.offset, used in mmap call below
    ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &myBuf->mreq);

    if (ret)
    {
        printf("Failed to map dumb buffer");
        return MAP_FAILED;
    }

    // drmSetMaster(fd);

    // //clear crtc


    drmModeSetCrtc(fd, crtc->crtc_id, *fb, 0, 0, &conn->connector_id, 1, mode);
    printf("creating buf w CRTC ID: %d\n", crtc->crtc_id);

    // drmDropMaster(fd);

    // Map memory region for DRM framebuffer using size and mapped offset of dumbbuffer
    myBuf->map = mmap(0, myBuf->crereq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, myBuf->mreq.offset );//* (1+id));

    if (myBuf->map == MAP_FAILED)
    {
        printf("map failed");
        return MAP_FAILED;
    }
    return myBuf->map;
}
void drm_unmap(struct buf_context *myBuf)
{
    munmap(myBuf->map, myBuf->crereq.size);
}

void print_info()
{
    //Printing information from libdrm structs, filled in drm_init()
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
}
int drm_close()
{

    drmModeFreeConnector(conn);
    drmModeFreeEncoder(encode);
    drmModeFreeResources(res);

    return 0;
}
void draw_pixel(int x, int y, uint32_t ARGB)
{


    //Local pointer to point to memory mapped display region
    uint32_t *pixelPtr;

    pixelPtr = (uint32_t * )(bufs[current_buff]->map);


    //Advance pixelPtr to correct row
    pixelPtr += mode->hdisplay * y;
    //Advance pixelPtr to correct column
    pixelPtr += x;

    *pixelPtr = ARGB;
}
void demo()
{
    int y, x;
    //Loop to iterate through rows
    for (y = 0; y < mode->vdisplay; y++)
    {
        //Loop to iterate through columns
        for (x = 0; x < mode->hdisplay; x++)
        {
            //Draw top 3rd of screen red
            if (x < (mode->hdisplay / 3))
            {
                draw_pixel(x, y, colors[0]);
            }
                //Draw middle 3rd of screen blue
            else if (x < (mode->hdisplay * 2 / 3))
            {
                draw_pixel(x, y, colors[1]);
            }
                //Draw bottom 3rd of screen green
            else
            {
                draw_pixel(x, y, colors[2]);
            }
        }
    }
}
void demo2()
{
    int y, x;
    //Loop to iterate through rows
    for (y = 0; y < mode->vdisplay; y++)
    {
        //Loop to iterate through columns
        for (x = 0; x < mode->hdisplay; x++)
        {
            //Draw top 3rd of screen red
            if (x < (mode->hdisplay / 3))
            {
                draw_pixel(x, y, colors[1]);
            }
                //Draw middle 3rd of screen blue
            else if (x < (mode->hdisplay * 2 / 3))
            {
                draw_pixel(x, y, colors[2]);
            }
                //Draw bottom 3rd of screen green
            else
            {
                draw_pixel(x, y, colors[0]);
            }
        }
    }
}

void pageFlip(){



    //printf("inside page flip\n");
    int ret;
    int fd = bufs[current_buff]->fd;
    //void *waiting;
    //unsigned int waiting(1);
    //printf("inside page flip CRTC ID: %d\n", crtc->crtc_id);


    uint32_t *fb = malloc(sizeof(uint32_t));
    //printf("fb id %d\n", *fb);
    fb = &bufs[current_buff]->fb;

    crtc = drmModeGetCrtc(fd, encode->crtc_id);
    ret = drmModeSetCrtc(bufs[current_buff]->fd, crtc->crtc_id,  *fb,  0, 0, &conn->connector_id, 1, mode);


    //ret = drmModePageFlip(myBuf->fd, crtc->crtc_id, myBuf->fb, DRM_MODE_PAGE_FLIP_ASYNC, waiting);
    if ( ret){
        printf("couldn't page flip\n");
        if (ret == -EINVAL)
        {
            printf("invalid crtc id\n");
        }
        else if (ret == -errno) {
            printf("other page flip error\n");
        }
    }

    if(current_buff == 0){
        //printf("switching, %d\n", current_buff);
        current_buff = 1;
    }
    else{
        //printf("switching, %d\n", current_buff);
        current_buff = 0;
    }
}

void changeActiveBuffer(){
    if(current_buff == 0){
        //printf("switching, %d\n", current_buff);
        current_buff = 1;
    }
    else{
        //printf("switching, %d\n", current_buff);
        current_buff = 0;
    }
}