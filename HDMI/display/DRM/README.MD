DRM requires /lib/usr/libdrm to be included in g++ or in makefile

for g++  add the argument "-I /usr/include/libdrm"
g++ -I/usr/include/libdrm drm_test.c  -o drm_test -ldrm