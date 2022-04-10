CFLAGS=`pkg-config --cflags gstreamer-1.0 gstreamer-app-1.0 gstreamer-plugins-base-1.0` -g -Wall -Werror --std=c99
LIBS = `pkg-config --libs gstreamer-1.0 gstreamer-app-1.0 gstreamer-plugins-base-1.0` -lgstvideo-1.0

all: image-out rx-example

rx-example: rx-example.c gstreamer-rx.o imagelib.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

gstreamer-rx.o: gstreamer-rx.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

image-out: image-out.c imagelib.o
	$(CC) $(CFLAGS) $^ -o $@

imagelib.o: imagelib.c imagelib.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm image-out rx-example *.o

format:
	clang-format -i *.c *.h

.PHONY: all clean format
