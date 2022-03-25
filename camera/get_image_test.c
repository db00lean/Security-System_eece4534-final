// Code references used for this file:
//      https://gist.github.com/jayrambhia/5866483
//      https://medium.com/@athul929/capture-an-image-using-v4l2-api-5b6022d79e1d

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

uint8_t *buffer;
int stream_on = 0;

// Wrapper for doing ioctl operations
static int xioctl(int fd, int request, void *arg)
{
    int r;

    do
        r = ioctl(fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

int print_capabilities(int fd)
{
    struct v4l2_capability caps = {0};
    if (xioctl(fd, VIDIOC_QUERYCAP, &caps) == -1)
    {
        perror("Querying Capabilities");
        return -1;
    }

    printf("Driver Caps:\n"
           "  Driver: \"%s\"\n"
           "  Card: \"%s\"\n"
           "  Bus: \"%s\"\n"
           "  Version: %d.%d\n"
           "  Capabilities: %08x\n",
           caps.driver,
           caps.card,
           caps.bus_info,
           (caps.version >> 16) && 0xff,
           (caps.version >> 24) && 0xff,
           caps.capabilities);

    struct v4l2_cropcap cropcap = {0};
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_CROPCAP, &cropcap))
    {
        perror("Querying Cropping Capabilities");
        return -1;
    }

    printf("Camera Cropping:\n"
           "  Bounds: %dx%d+%d+%d\n"
           "  Default: %dx%d+%d+%d\n"
           "  Aspect: %d/%d\n",
           cropcap.bounds.width, cropcap.bounds.height, cropcap.bounds.left, cropcap.bounds.top,
           cropcap.defrect.width, cropcap.defrect.height, cropcap.defrect.left, cropcap.defrect.top,
           cropcap.pixelaspect.numerator, cropcap.pixelaspect.denominator);

    struct v4l2_fmtdesc fmtdesc = {0};
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    char fourcc[5] = {0};
    char c, e;
    printf("  FMT : CE Desc\n--------------------\n");
    while (0 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
    {
        strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
        c = fmtdesc.flags & 1 ? 'C' : ' ';
        e = fmtdesc.flags & 2 ? 'E' : ' ';
        printf("  %s: %c%c %s\n", fourcc, c, e, fmtdesc.description);
        fmtdesc.index++;
    }

    return 0;
}

int set_format(int fd)
{
    struct v4l2_format format = {0};
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = 640;
    format.fmt.pix.height = 480;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    if (-1 == xioctl(fd, VIDIOC_S_FMT, &format))
    {
        perror("Setting Pixel Format");
        return -1;
    }

    char fourcc[5] = {0};
    strncpy(fourcc, (char *)&format.fmt.pix.pixelformat, 4);
    printf("Selected Camera Mode:\n"
           "  Width: %d\n"
           "  Height: %d\n"
           "  PixFmt: %s\n"
           "  Field: %d\n",
           format.fmt.pix.width,
           format.fmt.pix.height,
           fourcc,
           format.fmt.pix.field);

    return 0;
}

// allocate device buffers
int request_buffer(int fd, int count)
{
    struct v4l2_requestbuffers req = {0};
    req.count = count;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    // initialize memory mapped user pointer based I/O
    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req))
    {
        perror("Requesting Buffer");
        return -1;
    }
    return 0;
}

// query the buffer
int query_buffer(int fd)
{
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
    {
        perror("Querying Buffer");
        return 1;
    }

    // map buf.length bytes from fd to the buffer
    buffer = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    printf("Image Length: %d\n", buf.bytesused);

    return buf.length;
}

int start_streaming(int fd)
{
    unsigned int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
    {
        perror("Start Stream");
        return 1;
    }
}

int stop_streaming(int fd) {
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        perror("Stop Stream");
        return 1;
    }
}


int queue_buffer(int fd) {
    struct v4l2_buffer bufd = {0};
    bufd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufd.memory = V4L2_MEMORY_MMAP;
    bufd.index = 0;
    if (-1 == xioctl(fd, VIDIOC_QBUF, &bufd))
    {
        perror("Query Buffer");
        return 1;
    }
    return bufd.bytesused;
}

int dequeue_buffer(int fd) {
    struct v4l2_buffer bufd = {0};
    bufd.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufd.memory = V4L2_MEMORY_MMAP;
    bufd.index = 0;
    if (-1 == xioctl(fd, VIDIOC_DQBUF, &bufd))
    {
        perror("Dequeue buffer");
        return 1;
    }
    return bufd.bytesused;
}


// grab a frame and place it into a file
void grab_yuyvframe(int fd) 
{
    queue_buffer(fd);
    //Wait for io operation
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval tv = {0};
    tv.tv_sec = 2; //set timeout to 2 seconds
    int r = select(fd+1, &fds, NULL, NULL, &tv);
    if(-1 == r){
        perror("Waiting for Frame");
        exit(1);
    }

    // TODO: not sure this makes sense to do
    unsigned int size = query_buffer(fd);

    int file = open("output.yuy", O_WRONLY);
    write(file, buffer, size); //size is obtained from the query_buffer function
    dequeue_buffer(fd);
}


int main()
{
    // Note: this index could vary?
    int fd = open("/dev/video0", O_RDWR);
    if (fd == -1)
    {
        printf("Error opening video device");
        return 1;
    }

    // print capabilities
    if (print_capabilities(fd))
    {
        return 1;
    }

    if (set_format(fd))
        return 1;

    if (request_buffer(fd, 1))
        return 1;

    query_buffer(fd);

    if(start_streaming(fd))
        return 1;

    // for (int i = 0; i < 5; i++)
    // {
        grab_yuyvframe(fd);
    // }

    stop_streaming(fd);

    close(fd);
    return 0;
}