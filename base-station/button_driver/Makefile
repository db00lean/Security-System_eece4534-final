# Makefile for ZedBoard button driver

obj-m := zed_btns.o
ccflags-y += -DDEBUG
ESL_KERNEL_VERSION ?= 5.10.0-esl
ESL_ZEDBOARD_IP ?= 10.42.1.1
ifeq ($(OECORE_TARGET_SYSROOT),)
$(error "Can't determine SDK path. Verify that you have sourced the environment correctly.")
endif
KERNEL_SRC ?= $(OECORE_TARGET_SYSROOT)/lib/modules/$(ESL_KERNEL_VERSION)/build
ARCH ?= arm
CROSS_COMPILE ?= arm-linux-esl-gnueabi-
SRC := $(shell pwd)
UPLOAD_PATH ?= /home/root/

MODULE_OBJ:=$(obj-m:.o=.ko)

all: modules

modules:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_SRC) M=$(SRC)

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c *.mod
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
	