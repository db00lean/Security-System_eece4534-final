TARGET:=computer_vision.o
SRCS:=cv_main.c
OBJS:=$(SRCS:.c=.o)
LZED:=-lzed
LALSA:= -lasound -lpthread -lrt -ldl -lm

include ../zed.mk
.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LALSA) $(LZED)

%.o: %.c %.h
	$(CC) -c $<

%.o: %.c
	$(CC) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)

# TODO: Update Makefile to properly make the computer-vision project.
#       The current version is a stub from lab 4, and needs to be refitted for this project