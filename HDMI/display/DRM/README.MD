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
