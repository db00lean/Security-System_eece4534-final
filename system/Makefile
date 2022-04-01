# Makefile for system management server. 
# Run `make` or `make sysman_server` to build the entire server
# Run `make button_listener` to create the `button_listener` standalone executable.

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)
LIBS := -lpthread
CFLAGS := -O

.PHONY: sysman_server button_listener clean

sysman_server: management_server.o agg_detect.o button_client.o
	$(CC) -o sysman_server system_management_server.o aggregate_detect.o button_client.o $(LIBS)

sysman_client: client_zedboard/system_management_client.cpp
	(cd client_zedboard && g++ system_management_client.cpp -o sysman_client)

button_listener: button_client_standalone.o
	$(CC) -o $@ button_client_standalone.o $(LIBS)

management_server.o: system_management_server.c system_management.h aggregate_detect.h
	$(CC) $(CFLAGS) -c system_management_server.c system_management.h aggregate_detect.h

agg_detect.o: aggregate_detect.c aggregate_detect.h system_management.h
	$(CC) $(CFLAGS) -c aggregate_detect.c aggregate_detect.h system_management.h

button_client.o: button_client.c button_client.h
	$(CC) $(CFLAGS) -c button_client.c

button_client_standalone.o: button_client.c button_client.h
	$(CC) $(CFLAGS) -o button_client_standalone.o -c button_client.c -DBUTTON_CLIENT_MAIN

clean:
	rm -rf sysman_server button_listener *.o *.h.gch
	(cd client_zedboard && rm -rf sysman_client)