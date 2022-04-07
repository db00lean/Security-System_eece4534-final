DRM requires /lib/usr/libdrm to be included in g++ or in makefile

for g++  add the argument "-I /usr/include/libdrm"
g++ -I/usr/include/libdrm drm_test.c  -o drm_test -ldrm

## Tutorials
https://www.systutorials.com/docs/linux/man/7-drm-memory/
https://fs.emersion.fr/protected/presentations/present.html?src=kms-foss-north/index.md#26


## libdrm documentation 
https://docs.nvidia.com/jetson/l4t-multimedia/group__direct__rendering__manager.html#gab25f8928b38d02da6ed66a38f2802205

## libdrm struct info
https://01.org/linuxgraphics/gfx-docs/drm/drm-kms-properties.html

## Debian Man Page
https://manpages.debian.org/testing/libdrm-dev/drm.7.en.html


## Order of Opperations

First, we start with drm_open which just opens the /dev/dri/card0 character device with READ/WRITE capabilities

Then we initlize all our DRM variables with drm_init

drmModeGetRecourses  This is the top most layer, this gets all the avaible information about the GPU, including the connectors, encoders, CRTC, and a hand full of other information about the monitor

drmModeGetConnector gets all our connector information such as it's encoders

drmModeGetEncoder gets information about the encoder which holds information about the CRTC

drmModeGetCrtc gets the infromation about the CRTC which holds the image buffer we are sending it

conn->modes gets the infromation about the Mode

drmModeGetPlaneResources this is the only resource that isn't tired to the drmModeGetResources, currently returns that we have 0 planes to play with, going to look into why that is

The code then prints out a bunch of information based on the resrouces