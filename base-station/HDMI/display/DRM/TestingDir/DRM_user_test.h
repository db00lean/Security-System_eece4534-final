
#ifndef __DRM_USER__
#define __DRM_USER__
#include <stdint.h>


//Pointer to memory mapped region for writing to card
//void *map;

struct buf_context{
	void *map;
    uint32_t fb;
};




//Opens device at "/dev/dri/card0"
int drm_open();
//Initializes DRM -- uses libdrm to create structs with DRM device info
int drm_init(int fd);
//Maps device to virtual address
void *drm_map(int fd, struct buf_context *myBuf);
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
//void pageFlip(int fd, struct buf_context *bufs);


#endif
