CFLAGS=`pkg-config --cflags gstreamer-1.0 gstreamer-app-1.0` -g -Wall -Werror --std=c99
LIBS = `pkg-config --libs gstreamer-1.0 gstreamer-app-1.0`

all: image-out gstreamer-rx

gstreamer-rx: gstreamer-rx.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

image-out: image-out.c imagelib.o
	$(CC) $(CFLAGS) $^ -o $@

imagelib.o: imagelib.c imagelib.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm image-out gstreamer-rx *.o

format:
	clang-format -i *.c *.h

.PHONY: all clean format
