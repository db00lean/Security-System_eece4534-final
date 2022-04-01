CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

cv_main: cv_main.cpp 
	g++ -g -Wall -o computer_vision.o cv_main.cpp `pkg-config --cflags --libs opencv`

# test: test.cpp
	# g++ $(CFLAGS) $(LIBS) -o $@ $<

clean: 
	$(RM) computer_vision.o

# OLD MAKEFILE BELOW: Leaving as a reference for when we migrate to zedboard stuff.
# Need to fix cv_main.cpp compiling first before the bottom half can be reimplemented

# TARGET:=computer_vision.o
# SRCS:=cv_main.cpp
# OBJS:=$(SRCS:.c=.o)
# LZED:=-lzed
# LALSA:= -lasound -lpthread -lrt -ldl -lm

# # include ../zed.mk
# # .PHONY: clean

# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CC) -o $@ $^
# # $(CC) -o $@ $^ $(LALSA) $(LZED)

# %.o: %.cpp %.h
# 	$(CC) -c $<

# %.o: %.cpp
# 	$(CC) -c $<

# clean:
# 	rm -rf $(OBJS) $(TARGET)

# TODO: Update Makefile to properly make the computer-vision project.
#       The current version is a stub from lab 4, and needs to be refitted for this project