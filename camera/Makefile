TARGET:=get_image_test
SRCS:=get_image_test.c
OBJS:=$(SRCS:.c=.o)


ZED_LIB?= /usr/share/EECE4534/lib
ZED_INCLUDE?=/usr/share/EECE4534/include
INCLUDE_DIRS:=.. $(ZED_INCLUDE)
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g

ESL_KERNEL_VERSION ?= 5.10.0-esl
ifeq ($(OECORE_TARGET_SYSROOT),)
$(error "Can't determine SDK path. Verify that you have sourced the environment correctly.")
endif
KERNEL_SRC ?= $(OECORE_TARGET_SYSROOT)/lib/modules/$(ESL_KERNEL_VERSION)/build
CROSS_COMPILE ?= arm-linux-esl-gnueabi-

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c %.h
	$(CC) -c $<

%.o: %.c
	$(CC) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)