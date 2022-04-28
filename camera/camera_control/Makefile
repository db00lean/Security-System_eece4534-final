TARGET:=camera_control
SRCS:=camera_control.c
OBJS:=$(SRCS:.c=.o)

.PHONY: clean

camera_control: camera_control.c
	$(CC) -c camera_control.c

clean:
	rm -rf $(OBJS)