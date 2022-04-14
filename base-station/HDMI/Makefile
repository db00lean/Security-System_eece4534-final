TARGET:=main
SRCS:=main.c src/draw_bounding_box.c src/DRM_user.c src/drawtext.c src/font_8x8.c src/imagelib.c
OBJS:=$(SRCS:.c=.o)

GST_CFLAGS:= $(shell pkg-config --cflags gstreamer-1.0 gstreamer-app-1.0 gstreamer-plugins-base-1.0) -g -Wall -Werror --std=c99
GST_LIBS:= $(shell pkg-config --libs gstreamer-1.0 gstreamer-app-1.0 gstreamer-plugins-base-1.0) -lgstvideo-1.0

DRM_CFLAGS := $(shell pkg-config --cflags libdrm)
DRM_LDFLAGS := $(shell pkg-config --libs libdrm)

INCLUDE_DIRS:=.. $(ZED_INCLUDE) ./inc
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O1 $(DRM_CFLAGS) $(GST_CFLAGS)
LALSA:= -lasound -lpthread -lrt -ldl -lm

.PHONY: clean

all: $(TARGET) rtsp-things

# hack, we should make a nicer makefile
rtsp-things:
	$(MAKE) -f Makefile_rtsp

$(TARGET): $(OBJS)
	$(CC) $(DRM_LDFLAGS) $(GST_LIBS) -o $@ $^ -lm

%.o: %.c %.h
	$(CC) $(CFLAGS) $(GST_CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(TARGET) *.o

.PHONY: all clean rtsp-things
