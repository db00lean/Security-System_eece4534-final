# Makefile for system management server. 
# Run `make` or `make sysman_server` to build the entire server
# 129.10.156.142

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)
LIBS := -lpthread -lczmq -lzmq -lm $(shell pkg-config --libs gstreamer-1.0 gstreamer-app-1.0 gstreamer-plugins-base-1.0) -lgstvideo-1.0
CFLAGS := -O -g -Wall $(shell pkg-config --cflags gstreamer-1.0 gstreamer-app-1.0 gstreamer-plugins-base-1.0) --std=c99
DRM_LDFLAGS := $(shell pkg-config --libs libdrm)
GST_LIBS:= $(shell pkg-config --libs gstreamer-1.0 gstreamer-app-1.0 gstreamer-plugins-base-1.0) -lgstvideo-1.0

.PHONY: sysman_server button_listener clean

sysman_server: system_management_server.o agg_detect.o button_client.o
	(cd button_driver && make)
	(cd ../network && make server.o)
	(cd HDMI && make hdmi-lib.a)
	$(CC) -o sysman_server system_management_server.o aggregate_detect.o button_client.o ../network/server.o ../network/client.o ../network/packet.o ./HDMI/hdmi-lib.a $(DRM_LDFLAGS) $(GST_LIBS) $(LIBS)

button_listener: button_client_standalone.o
	$(CC) -o $@ button_client_standalone.o $(BTN_LIBS)

system_management_server.o: system_management_server.c aggregate_detect.h
	$(CC) $(CFLAGS) -c system_management_server.c

agg_detect.o: aggregate_detect.c aggregate_detect.h
	$(CC) $(CFLAGS) -c aggregate_detect.c

button_client.o: button_client.c
	$(CC) $(CFLAGS) -c button_client.c -DDEBUG

clean:
	(cd button_driver && make clean)
	(cd ../network && make clean)
	(cd HDMI && make clean)
	rm -rf sysman_server button_listener *.o *.h.gch

