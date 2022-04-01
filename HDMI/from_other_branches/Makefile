VERSION = 1
PATCHLEVEL = 0
SUBLEVEL = 0

TARGET:=drm_test
SRCS:=/from_other_branches/drm_test.c
OBJS:=$(SRCS:.c=.o)


override CFLAGS += -DVERSION=$(VERSION)
override CFLAGS += -DPATCHLEVEL=$(PATCHLEVEL)
override CFLAGS += -DSUBLEVEL=$(SUBLEVEL)
override CFLAGS += -DVERSION_NAME=\"$(NAME)\"

INCLUDE_DIRS:=.. $(ZED_INCLUDE)
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0
LALSA:= -lasound -lpthread -lrt -ldl -lm

.PHONY: clean

all: $(PROGS)


test_draw_bounding_boxes: test_draw_bounding_boxes.o draw_bounding_box.c

test_draw_image: test_draw_image.o imagelib.c draw_bounding_box.c

drm_test: drm_test.o src/draw_bounding_box.c

clean:
	rm -rf $(OBJS) $(TARGET) *.o