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
#include "../inc/draw_bounding_box.h"
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

//Function prototypes
int drm_open();
int drm_init(int fd);
int drm_close();
void draw_pixel(int x, int y, uint32_t ARGB);
void demo(int resolution);

//Defining constants for colors according to default pixel format -- 32 bit word with transparency, red, green, and blue values
uint32_t const red = (0xff << 16);
uint32_t const green = (0xff << 8);
uint32_t const blue = (0xff);
uint32_t const colors[] = {red, green, blue};

int main()
{

    int ret;

    //Open /dev/dri/card0
    int fd = drm_open();

    if (fd == -1)
    {
        printf("fd failed to open");
        return -1;
    }

    //Initialize drm and error check
    if (drm_init(fd) == -1)
    {
        printf("Error with init DRM");
        return -1;
    }

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
        return ret;
    }
    // create DRM FB using information within crereq, populated by drmIoctl call above
    ret = drmModeAddFB(fd, (uint32_t)crereq.width, (uint32_t)crereq.height, 24,
                       crereq.bpp, crereq.pitch, crereq.handle, &fb);

    if (ret)
    {
        printf("Failed to create DRM buffer\n");
        printf("ret: %d\n", ret);
        return ret;
    }

    //printf("fb: %d\n", fb);
    // wont work because fb is uint32_t in scope, doesn't refer to struct
    // printf("bits per pixel %d\n",(int)fb->bpp);

    // Clear mreq
    memset(&mreq, 0, sizeof(mreq));

    // Set memory mapping handle equal to the handle of the dumb fb just created
    mreq.handle = crereq.handle;
    // Map dumb buffer based on mreq.handle
    // This Ioctl call populates mreq.offset, used in mmap call below
    ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);

    if (ret)
    {
        printf("Failed to set map dumb");
        return -1;
    }

    // Map memory region for DRM framebuffer using size and mapped offset of dumbbuffer
    map = mmap(0, crereq.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);

    if (map == MAP_FAILED)
    {
        printf("map failed");
        return -1;
    }

    memset(map, 0, crereq.size);


    int i;
    int count = 0;
    // float resolution = mode->vdisplay * mode->hdisplay;
    int resolution = mode->vdisplay * mode->hdisplay;
    printf("size %d\n", crereq.size);
    printf("resolution %d\n", resolution);
    printf("calculated resolution %d\n", mode->vdisplay * mode->hdisplay);
    printf("test\n");
    demo(resolution);

    printf("draw image start\n");

    int IMG_W = 1920;
    int IMG_H = 1080;
    struct image *img = create_image(IMG_W, IMG_H);

    for (int ii = 0; ii < IMG_H; ii++)
    { // rows
        for (int jj = 0; jj < IMG_W; jj++)
        { // columns
            //  color within pixel: +0,1,2
            img->buf[PIXEL(jj, ii) + 0] = 255 * jj / 3 / IMG_W;
            img->buf[PIXEL(jj, ii) + 1] = 0;
            img->buf[PIXEL(jj, ii) + 2] = 255 * ii / IMG_H;
        }
    }
    // draw image to screen using draw pixel
    //  for each box, draw a rectangle bounding box
    for (int x = 0; x < IMG_W; x++)
    {
        for (int y = 0; y < IMG_H; y++)
        {
            unsigned int color = (img->buf[x * 3 + y * 3 * IMG_W] << 16) | (img->buf[x * 3 + y * 3 * IMG_W + 1] << 8) | (img->buf[x * 3 + y * 3 * IMG_W + 2] << 0);
            draw_pixel(x, y, color);
        }
    }
    printf("End of draw image\n");

    printf("Drawing with draw pixel drm\n");
    // demo draw pixel bounding box
    // Using cv data struct format
    struct cv_data cvdata1;
    // Fill with data for now
    for (int i = 0; i < MAX_B_BOXES; i++)
    {
        cvdata1.num_bbox++; // increase number of boxes
        // fill in coordinates
        cvdata1.box_data[i].x_coord = 40 + (i * 60);
        cvdata1.box_data[i].y_coord = 40;
        cvdata1.box_data[i].x_len = 50;
        cvdata1.box_data[i].y_len = 50;
    }

    //   fb_open();
    // for each box, draw a rectangle bounding box
    for (int i = 0; i < cvdata1.num_bbox; i++)
    {
        unsigned int color = 0xFF00FF;
        // printf("draw pixel %d %d %d %d %x\n", cvdata1.box_data->x_coord, cvdata1.box_data->y_coord, cvdata1.box_data->x_len, cvdata1.box_data->y_len, color);
        draw_boundingbox(cvdata1.box_data[i].x_coord, cvdata1.box_data[i].y_coord, cvdata1.box_data[i].x_len, cvdata1.box_data[i].y_len, color);
    }
    printf("end Drawing with draw pixel drm\n");

    drmSetMaster(fd);

    // clear crtc
    drmModeSetCrtc(fd, crtc->crtc_id, 0, 0, 0, NULL, 0, NULL);

    drmModeSetCrtc(fd, crtc->crtc_id, fb, 0, 0, &conn->connector_id, 1, mode);

    drmDropMaster(fd);

    //Get plane information
    drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
    drmModePlaneRes *planes = drmModeGetPlaneResources(fd);
    printf("num planes %d\n", planes->count_planes);

    //Hang until user presses a key
    getchar();

    drm_close();

    return 0;
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

int drm_close()
{

    drmModeFreeConnector(conn);
    drmModeFreeEncoder(encode);
    drmModeFreeResources(res);

    return 0;
}

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
//Draw a pixel at (x, y) on screen with pixel information contained in 32-bit word ARGB
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
//Demo function to fill the screen with 3 rectangles
void demo(int resolution)
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
