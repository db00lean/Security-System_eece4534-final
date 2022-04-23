
#ifndef __DRM_USER__
#define __DRM_USER__
#include <stdint.h>

#define PIXEL(x, y) ((y * IMG_W * 3) + (x * 3))

// This is the default and only card on the zedboard
#define CARD_PATH "/dev/dri/card0"

//Opens device at "/dev/dri/card0"
int drm_open();
//Initializes DRM -- uses libdrm to create structs with DRM device info
int drm_init(int fd);
//Maps device to virtual address
void *drm_map(int fd);
//Frees virtual memory space mapped to DRM device
void drm_unmap();
//Print info obtained in drm_init()
void print_info();
//Destroy DRM info structs
int drm_close();
//Draw pixel at coordinates (x, y) with alpha "A", red "R", green "G", blue "B"
void draw_pixel(int x, int y, uint32_t ARGB);
//Demo function to fill the screen with 3 rectangles
void demo();


#endif
