CFLAGS=`pkg-config --cflags --libs gstreamer-1.0` -g

image-out: image-out.c imagelib.c
	gcc $(CFLAGS) imagelib.c image-out.c -o image-out
