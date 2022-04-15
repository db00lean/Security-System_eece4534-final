# Makefile for system management server. 
# Run `make` or `make sysman_server` to build the entire server
# Run `make button_listener` to create the `button_listener` standalone executable.

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)
LIBS := -lpthread -lczmq -lzmq -lm
CFLAGS := -O -g -Wall
DRM_LDFLAGS := $(shell pkg-config --libs libdrm)

.PHONY: sysman_server button_listener clean

sysman_server: management_server.o agg_detect.o button_client.o
	(cd ../network && make server.o)
	(cd HDMI && make hdmi-lib.a)
	$(CC) -o sysman_server system_management_server.o aggregate_detect.o button_client.o ../network/server.o ../network/client.o ../network/packet.o ./HDMI/hdmi-lib.a $(DRM_LDFLAGS) $(LIBS)

button_listener: button_client_standalone.o
	$(CC) -o $@ button_client_standalone.o $(LIBS)

management_server.o: system_management_server.c aggregate_detect.h
	$(CC) $(CFLAGS) -c system_management_server.c

agg_detect.o: aggregate_detect.c aggregate_detect.h
	$(CC) $(CFLAGS) -c aggregate_detect.c

button_client.o: button_client.c
	$(CC) $(CFLAGS) -c button_client.c -DDEBUG

button_client_standalone.o: button_client.c
	$(CC) $(CFLAGS) -o button_client_standalone.o -c button_client.c -DBUTTON_CLIENT_MAIN

clean:
	(cd ../network && make clean)
	(cd HDMI && rm -f hdmi-lib.a)
	rm -rf sysman_server button_listener *.o *.h.gch

