TARGET:=test_draw_bounding_boxes
SRCS:=test_draw_bounding_boxes.c
OBJS:=$(SRCS:.c=.o)

INCLUDE_DIRS:=.. $(ZED_INCLUDE)
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0
LALSA:= -lasound -lpthread -lrt -ldl -lm

.PHONY: clean

all: $(PROGS)


boundingbox: test_draw_bounding_boxes.o draw_bounding_box.h

drawimage: test_draw_image.o draw_bounding_box.h

clean:
	rm -rf $(OBJS) $(TARGET) *.o









