TARGET:=main
SRCS:=main.c src/draw_bounding_box.c src/DRM_user.c src/drawtext.c src/font_8x8.c 
OBJS:=$(SRCS:.c=.o)

DRM_CFLAGS := $(shell pkg-config --cflags libdrm)
DRM_LDFLAGS := $(shell pkg-config --libs libdrm)

INCLUDE_DIRS:=.. $(ZED_INCLUDE) /inc
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0 $(DRM_CFLAGS)
LALSA:= -lasound -lpthread -lrt -ldl -lm

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(DRM_LDFLAGS) $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -rf $(OBJS) $(TARGET) *.o