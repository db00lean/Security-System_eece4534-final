TARGET:=main_drm_test
SRCS:=main_drm_test.c DRM_user_test.c 
OBJS:=$(SRCS:.c=.o)
ESL_ZEDBOARD_IP:=129.10.156.135
UPLOAD_PATH:=/~

DRM_CFLAGS := $(shell pkg-config --cflags libdrm)
DRM_LDFLAGS := $(shell pkg-config --libs libdrm)

INCLUDE_DIRS:=. .. $(ZED_INCLUDE)
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0 $(DRM_CFLAGS)

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(DRM_LDFLAGS) $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -rf $(OBJS) $(TARGET)
upload: all
	scp $(TARGET) root@$(ESL_ZEDBOARD_IP):$(UPLOAD_PATH)