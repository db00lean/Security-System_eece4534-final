VERSION = 1
PATCHLEVEL = 0
SUBLEVEL = 0

TARGET:=boundingbox
SRCS:=boundingbox.c
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

.c.o: common.h font.h

fb-test: fb-test.o common.o font_8x8.c

boundingbox: boundingbox.o drawpixel.h common.o font_8x8.c

clean:
	rm -rf $(OBJS) $(TARGET) *.o fb-test









