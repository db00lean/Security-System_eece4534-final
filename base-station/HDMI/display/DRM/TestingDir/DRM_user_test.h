
#ifndef __DRM_USER__
#define __DRM_USER__
#include <stdint.h>
#include <stdio.h>


//Struct to hold frame contents
//Struct to hold frame contents
struct buf_context{
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint32_t size;
	uint32_t handle;
	void *bufmap;
	uint32_t fb;
    
};

//Pointer to memory mapped region for writing to card
void *map;

//Opens device at "/dev/dri/card0"
int drm_open();
//Initializes DRM -- uses libdrm to create structs with DRM device info
int drm_init(int fd);
//Maps device to virtual address
void *drm_map(int fd);
//Makes a new FB
int makeFB(int fd, struct buf_context* buf);

//Frees virtual memory space mapped to DRM device
void drm_unmap();
//Print info obtained in drm_init()
void print_info();
//Destroy DRM info structs
int drm_close();
//Draw pixel at coordinates (x, y) with alpha "A", red "R", green "G", blue "B"
void draw_pixel(int x, int y, uint32_t ARGB, void* bufmap);
//Demo function to fill the screen with 3 rectangles
void demo(struct buf_context buf);
//demo 2
void demo2();
//PageFlip
void pageFlip(int fd, struct buf_context* bufs);

/*

    drmSetMaster(fd);

    // clear crtc
    drmModeSetCrtc(fd, crtc->crtc_id, 0, 0, 0, NULL, 0, NULL);

    drmModeSetCrtc(fd, crtc->crtc_id, fb, 0, 0, &conn->connector_id, 1, mode);

    drmDropMaster(fd);
*/


#endif
