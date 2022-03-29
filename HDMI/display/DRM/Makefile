TARGET:=drm_test
SRCS:=drm_test.c
OBJS:=$(SRCS:.c=.o)

ZED_LIB?= ../libzed-bin/lib
ZED_INCLUDE?= ../libzed-bin/include

INCLUDE_DIRS:=.. $(ZED_INCLUDE) /usr/include/libdrm
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0
#LZED:=-lzed -L $(ZED_LIB)
LZED:=-lzed
LALSA:= -lasound -lpthread -lrt -ldl -lm

#include zed.mk
.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LALSA) $(LZED)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)