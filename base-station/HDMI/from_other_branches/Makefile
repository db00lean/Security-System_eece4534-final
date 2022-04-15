TARGET:=drm_test
SRCS:=drm_test.c ../src/draw_bounding_box.c imagelib.c
OBJS:=$(SRCS:.c=.o)

DRM_CFLAGS := $(shell pkg-config --cflags libdrm)
DRM_LDFLAGS := $(shell pkg-config --libs libdrm)

INCLUDE_DIRS:=.. $(ZED_INCLUDE)
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0 $(DRM_CFLAGS)

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(DRM_LDFLAGS) $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -rf $(OBJS) $(TARGET)