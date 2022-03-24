//First test /dev/fb0 by issuing "cat /dev/urandom > /dev/fb0", should be static
//Drawing blue rectangle by following
//https://www.youtube.com/watch?v=x1oXByIJcHU see 2:20
//Using mmap process from lab1 gpio.c
//129.10.156.116
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>


int main() {
int fd;
int *fbmem;
//Values to determine frame and rectangle dimensions
int res_width = 1024, res_height = 768, bytes_per_pixel = 3, rect_width=360, rect_height=480;

//Open device file and mmap it
fd = open("/dev/fb0", O_RDWR);
fbmem = mmap(NULL, res_width*res_height*bytes_per_pixel,  PROT_WRITE, MAP_SHARED, fd, 0);

if (fbmem == MAP_FAILED)
  {
    printf("map failed\n");
    return -1;
  }

printf("blue pixel\n");
int i;
//Using arbitrary bounds to write a blue horizontal line on screen
for (i =24000; i<27000; i++)
{
  fbmem[i*3] = 0; //blue
  fbmem[i*3+1] = 255; //green
  fbmem[i*3+2] = 0; //red
}

//Example code causing segfault -- explore later

// fbmem += 200 * 1024 * 3 + 100 * 3; //jump to first pixel -- play with values

// int x, y;
// //Loop to draw blue rectangle
// for (y = 0; y < rect_width; y++){
//     for (x = 0; x < rect_height; x++) {
//       printf("before write blue\n");
//         fbmem[x*3] = 255; //blue
//         fbmem[x*3+1] = 0; //green
//         fbmem[x*3+2] = 0; //red
//         printf("after write red\n");
//     }
// }

}