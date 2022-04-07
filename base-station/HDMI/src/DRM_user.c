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

#include "cv_structs.h"
#include "draw_bounding_box.h"
#include "imagelib.h"



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

//Pointer to memory mapped region for writing to card
void *map;

//Defining constants for colors according to default pixel format -- 32 bit word with transparency, red, green, and blue values
uint32_t const red = (0xff << 16);
uint32_t const green = (0xff << 8);
uint32_t const blue = (0xff);
uint32_t const colors[] = {red, green, blue};

int drm_open()
{

    int fd, flags;

    // Opening cards
    fd = open("/dev/dri/card0", O_RDWR);

    if (fd == NULL)
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

    // set "struct drmModeModeInfo" to the "struct drmModeModeInfo" member contained within "drmModeConnector"
    mode = conn->modes;

    return 0;
}
void *drm_map()
{
    // 32 bit memory location to store address of framebuffer
    uint32_t fb;

    // struct to create dumb buffer
    struct drm_mode_create_dumb crereq;

    // struct to create memory mapping for dumb buffer
    struct drm_mode_map_dumb mreq;

    // struct to destroy dumb buffer
    struct drm_mode_destroy_dumb dreq;

    // clear crereq before setting members
    memset(&crereq, 0, sizeof(crereq));

    // set members of crereq based on members of "drmModeModeInfo" obtained in drm_init()
    crereq.height = mode->vdisplay;
    crereq.width = mode->hdisplay;
    crereq.bpp = 32;

    // create dumb DRM based on crereq members -- "handle, pitch, size will be returned", members of crereq
    ret = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &crereq);

    if (ret)
    {
        printf("Failed to make dumb buffer");
        return MAP_FAILED;
    }
    // create DRM FB using information within crereq, populated by drmIoctl call above
    ret = drmModeAddFB(fd, (uint32_t)crereq.width, (uint32_t)crereq.height, 24,
                       crereq.bpp, crereq.pitch, crereq.handle, &fb);

    if (ret)
    {
        printf("Failed to create DRM buffer\n");
        printf("ret: %d\n", ret);
        return MAP_FAILED;
    }

    // Clear mreq
    memset(&mreq, 0, sizeof(mreq));

    // Set memory mapping handle equal to the handle of the dumb fb just created
    mreq.handle = crereq.handle;
    // Map dumb buffer based on mreq.handle
    // This Ioctl call populates mreq.offset, used in mmap call below
    ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);

    if (ret)
    {
        printf("Failed to map dumb buffer");
        return MAP_FAILED;
    }

    drmSetMaster(fd);

    // clear crtc
    drmModeSetCrtc(fd, crtc->crtc_id, 0, 0, 0, NULL, 0, NULL);

    drmModeSetCrtc(fd, crtc->crtc_id, fb, 0, 0, &conn->connector_id, 1, mode);

    drmDropMaster(fd);

    // Map memory region for DRM framebuffer using size and mapped offset of dumbbuffer
    map = mmap(0, crereq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);

    if (map == MAP_FAILED)
    {
        printf("map failed");
        return MAP_FAILED;
    }
    return map;
}
void drm_unmap()
{
    munmap(map, crereq.size);
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
    pixelPtr = (uint32_t *)(map);

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
            if (y < (mode->vdisplay / 3))
            {
                draw_pixel(x, y, colors[0]);
            }
            //Draw middle 3rd of screen blue
            else if (y < (mode->vdisplay * 2 / 3))
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
