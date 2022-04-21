/*
    Provide basic control functionality for UVC webcam
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h> /* getopt_long() */

#include <fcntl.h> /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h> // for USB bus reset

#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

static char *dev_name;
static int fd = -1;


struct control_state
{

};

static void errno_exit(const char *s)
{
    fprintf(stderr, "%s error %d, %s\\n", s, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

// Wrapper for doing ioctl operations
static int xioctl(int fh, int request, void *arg)
{
    int r;

    do
    {
        r = ioctl(fh, request, arg);
    } while (-1 == r && EINTR == errno);

    return r;
}

static void close_device(void)
{
    if (-1 == close(fd))
        errno_exit("close");

    fd = -1;
}

static void open_device(void)
{
    struct stat st;

    // check file status
    if (-1 == stat(dev_name, &st))
    {
        fprintf(stderr, "Cannot identify '%s': %d, %s\\n",
                dev_name, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // check that the file is a character device file
    if (!S_ISCHR(st.st_mode))
    {
        fprintf(stderr, "%s is no device\n", dev_name);
        exit(EXIT_FAILURE);
    }

    fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

    if (-1 == fd)
    {
        fprintf(stderr, "Cannot open '%s': %d, %s\\n",
                dev_name, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

// hflip
void horizontal_flip()
{
    struct v4l2_control v;

    if (-1 == xioctl(fd, V4L2_CID_HFLIP, val))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_HFLIP\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_HFLIP");
        }
    }
}

// vlip
void vertical_flip()
{
    if (-1 == xioctl(fd, V4L2_CID_VFLIP, val))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_VFLIP\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_VFLIP");
        }
    }
}

// rotate
void rotate_video()
{

}

// brightness
void set_brightness()
{

}

// contrast
void set_contrast()
{

}


// pan (horizontal movement) by val
//  - negative val: move left
//  - positive val: move right
//  - zero val: no motion
void pan_relative(int val)
{
    if (-1 == xioctl(fd, V4L2_CID_PAN_RELATIVE, val))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_PAN_RELATIVE\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_PAN_RELATIVE");
        }
    }
}


// tilt (vertical movement) by val
//  - negative val: move down
//  - positive val: move up
//  - zero val: no motion
void tilt_relative(int val)
{
    if (-1 == xioctl(fd, V4L2_CID_TILT_RELATIVE, val))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_TILT_RELATIVE\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_TILT_RELATIVE");
        }
    }
}

// reset the horizontal and vertical position of the camera back to the default position
void reset_position()
{
    // TODO: not sure argument needs to be specified for button-type controls?
    if (-1 == xioctl(fd, V4L2_CID_TILT_RESET, 1))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_TILT_RESET\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_TILT_RESET");
        }
    }
    if (-1 == xioctl(fd, V4L2_CID_PAN_RESET, 1))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_PAN_RESET\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_PAN_RESET");
        }
    }
}


// xoom by val
//  - negative val: zoom out
//  - positive val: zoom in
//  - zero val: no change
void zoom_relative(int val)
{
    if (-1 == xioctl(fd, V4L2_CID_ZOOM_RELATIVE, val))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_TILT_RELATIVE\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_TILT_RELATIVE");
        }
    }
}

// reset to default zoom position
void zoom_reset()
{
    // TODO: not yet sure what the default zoom value is. Documentation says the value is driver-specific
    // so we will have to use VIDIOC_QUERY_EXT_CTRL to query and get the default value to find this
    if (-1 == xioctl(fd, V4L2_CID_TILT_RELATIVE, 0))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support V4L2_CID_TILT_RELATIVE\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("V4L2_CID_TILT_RELATIVE");
        }
    }
}


// reset USB bus for the device
void reset_usb_bus()
{
    if (-1 == xioctl(fd, USBDEVFS_RESET, 0))
    {
        if (EINVAL == errno)
        {
            fprintf(stderr, "%s doesn't support USBDEVFS_RESET\\n",
                    dev_name);
            exit(EXIT_FAILURE);
        }
        else
        {
            errno_exit("USBDEVFS_RESET");
        }
    }
}

// print info about supported controls (min, max, default values for controls)
void print_controls_info()
{

}


// NOTE: disable/enable stream can also be provided, but probably should be handled via gstreamer server

int main(int argc, char **argv)
{
    dev_name = "/dev/video0";

    open_device();
    // todo: do we need to call VIDIOC_QUERYCAP ioctl after opening the device?
    print_controls_info();

    /* test vwhatever controls from above */

    close_device();
    fprintf(stderr, "\\n");
    return 0;
}