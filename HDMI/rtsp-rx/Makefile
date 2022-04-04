CFLAGS=`pkg-config --cflags gstreamer-1.0 gstreamer-app-1.0` -g -Wall -Werror
LIBS = `pkg-config --libs gstreamer-1.0 gstreamer-app-1.0`

all: image-out gstreamer-rx

image-out: image-out.c imagelib.c
	gcc $(CFLAGS) imagelib.c image-out.c -o image-out $(LIBS)

gstreamer-rx: gstreamer-rx.c gstreamer-rx.c
	gcc $(CFLAGS) gstreamer-rx.c -o gstreamer-rx $(LIBS)
