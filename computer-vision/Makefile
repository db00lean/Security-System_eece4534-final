# CFLAGS = `pkg-config --cflags opencv`
# LIBS = `pkg-config --libs opencv`

# cv_main: cv_main.cpp 
# 	g++ -g -Wall -o computer_vision.o cv_main.cpp `pkg-config --cflags --libs opencv`

# time_test: cv_time_test.cpp 
# 	g++ -g -Wall -o computer_vision.o cv_main.cpp `pkg-config --cflags --libs opencv`

# # test: test.cpp
# 	# g++ $(CFLAGS) $(LIBS) -o $@ $<

# clean: 
# 	$(RM) computer_vision.o

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


TARGET:=cv_test
SRCS:=cv_time_test.cpp
OBJS:=$(SRCS:.c=.o)

CV_CFLAGS := $(shell pkg-config --cflags opencv)
CV_LDFLAGS := $(shell pkg-config --libs opencv)

# CFLAGS = `pkg-config --cflags opencv`
# LIBS = `pkg-config --libs opencv`

INCLUDE_DIRS:=.. $(ZED_INCLUDE) /inc
CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0 $(DRM_CFLAGS)
LALSA:= -lasound -lpthread -lrt -ldl -lm

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(DRM_LDFLAGS) $^ -lm

%.o: %.cpp %.h
	$(CXX) $(CFLAGS) -c $< 

clean:
	rm -rf $(OBJS) $(TARGET) *.o