//First test /dev/fb0 by issuing "cat /dev/urandom > /dev/fb0", should be static
//Drawing blue rectangle by following
//https://www.youtube.com/watch?v=x1oXByIJcHU see 2:20
//Using mmap process from lab1 gpio.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>


int main() {
int fd;
int *fbmem;
//Values to determine frame and rectangle dimensions
int res_width = 1024, res_height = 768, bytes_per_pixel = 3, rect_width=360, rect_height=480;
fd = open("/dev/fb0", O_RDWR);
fbmem = mmap(NULL, res_width*res_height*bytes_per_pixel,  PROT_WRITE, MAP_SHARED, fd, 0);

if (fbmem == MAP_FAILED)
  {
    printf("map failed");
    return -1;
  }

fbmem += 200 * 1024 * 3 + 100 * 3; //jump to first pixel -- play with values

int x, y;
//Loop to draw blue rectangle
for (x = 0; x < rect_width; x++){
    for (y = 0; y < rect_height; y++) {
        fbmem[x*3] = 255; //blue
        fbmem[x*3+1] = 0; //green
        fbmem[x*3+2] = 0; //red
    }
}

}