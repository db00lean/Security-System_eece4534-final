## dumb makefile
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)
LIBS := -lpthread

sysman_server: management_server.o agg_detect.o button_client.o
	gcc -o sysman_server system_management_server.o aggregate_detect.o button_client.o $(LIBS)

# $(OBJS): $(SRCS) $(HDRS)
# 	gcc -c -o $@ $<

management_server.o: system_management_server.c system_management.h aggregate_detect.h
	gcc -c system_management_server.c system_management.h aggregate_detect.h

agg_detect.o: aggregate_detect.c aggregate_detect.h system_management.h
	gcc -c aggregate_detect.c aggregate_detect.h system_management.h

button_client.o: button_client.h
	gcc -c button_client.c button_client.h

clean:
	rm -rf sysman_server *.o *.h.gch