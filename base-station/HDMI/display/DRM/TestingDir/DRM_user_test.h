
#ifndef __DRM_USER__
#define __DRM_USER__
#include <stdint.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <errno.h>




//Pointer to memory mapped region for writing to card
//void *map;

struct buf_context{
	void *map;
	int fd;
    uint32_t fb;
	// struct to create dumb buffer
	struct drm_mode_create_dumb crereq;

	// struct to create memory mapping for dumb buffer
	struct drm_mode_map_dumb mreq;

	// struct to destroy dumb buffer
	struct drm_mode_destroy_dumb dreq;
};




//Opens device at "/dev/dri/card0"
int drm_open();
//Initializes DRM -- uses libdrm to create structs with DRM device info
int drm_init(int fd);
//Maps device to virtual address
void *drm_map(int fd, struct buf_context *myBuf, int id);
//Frees virtual memory space mapped to DRM device
void drm_unmap(struct buf_context *myBuf);
//Print info obtained in drm_init()
void print_info();
//Destroy DRM info structs
int drm_close();
//Draw pixel at coordinates (x, y) with alpha "A", red "R", green "G", blue "B"
void draw_pixel(int x, int y, uint32_t ARGB, struct buf_context *myBuf);
//Demo function to fill the screen with 3 rectangles
void demo();

//page flip
void pageFlip(int fd, struct buf_context *bufs);


#endif
