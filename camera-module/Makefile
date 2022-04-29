<<<<<<< HEAD
libs:= -lpthread -lzmq -lczmq $(shell pkg-config --libs gstreamer-1.0 gstreamer-rtsp-server-1.0 opencv4)
CFLAGS:=-g $(shell pkg-config --cflags gstreamer-1.0 gstreamer-rtsp-server-1.0 opencv4) -O

main: system_management_client.o stream.o cv_main.o
	(cd ../network && make cplus)
	${CXX} -o main system_management_client.o stream.o cv_main.o ../network/client.o ../network/packet.o ${libs}

system_management_client.o: system_management_client.cpp
	${CXX} -c system_management_client.cpp ${CFLAGS}

stream.o: stream.cpp
	${CXX} -c stream.cpp ${CFLAGS}

cv_main.o: cv_main.cpp
	${CXX} -c cv_main.cpp ${CFLAGS}

clean:
	rm -f main *.o

=======
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

cv_main: cv_main.cpp 
	g++ -g -Wall -o computer_vision.o cv_main.cpp `pkg-config --cflags --libs opencv` -ldarknet

time_test: cv_time_test.cpp 
	g++ -g -Wall -o computer_vision.o cv_main.cpp `pkg-config --cflags --libs opencv`

jack: jack_scratch.cpp
	g++ -g -Wall -o jack.o jack_scratch.cpp -ldarknet

# test: test.cpp
	# g++ $(CFLAGS) $(LIBS) -o $@ $<

clean: 
	$(RM) computer_vision.o

# # export CPLUS_INCLUDE_PATH=/home/johnp/eece4534sdk/sysroots/cortexa9t2hf-neon-esl-linux-gnueabi/usr/include/opencv4
# TARGET:=output_cv
# SRCS:=cv_main.cpp
# TARGET:=time_output_cv
# SRCS:=cv_time_test.cpp
# OBJS:=$(SRCS:.cpp=.o)

# CV_CFLAGS := $(shell pkg-config --cflags opencv4)
# CV_LDFLAGS := $(shell pkg-config --libs opencv4)

# # CV_CFLAGS = -I/home/johnp/eece4534sdk/sysroots/cortexa9t2hf-neon-esl-linux-gnueabi/usr/include/opencv4/opencv2
#  #-I/home/johnp/eece4534sdk/sysroots/cortexa9t2hf-neon-esl-linux-gnueabi/usr/include/opencv4/
# # CV_LDFLAGS := $(shell pkg-config --libs opencv4)

# # CFLAGS = `pkg-config --cflags opencv`
# # LIBS = `pkg-config --libs opencv`

# INCLUDE_DIRS:=.. $(ZED_INCLUDE) /inc
# # CFLAGS:=$(foreach incdir, $(INCLUDE_DIRS), -I$(incdir)) -g -O0 $(CV_CFLAGS)
# CFLAGS:=-g -O0 $(CV_CFLAGS)
# LALSA:= -lasound -lpthread -lrt -ldl -lm

# .PHONY: clean

# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CXX) -o $@ $(CV_LDFLAGS) $^ -lm

# %.o: %.cpp %.h
# 	$(CXX) $(CFLAGS) -c $< 

# clean:
# 	rm -rf $(OBJS) $(TARGET) *.o

########################################### Uncomment everything above

# CFLAGS = `pkg-config --cflags opencv`
# LIBS = `pkg-config --libs opencv`

# cv_main: cv_main.cpp 
# 	g++ -g -Wall -o computer_vision.o cv_main.cpp `pkg-config --cflags --libs opencv`

# time_test: cv_time_testgit .cpp 
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
>>>>>>> 14423299e8b4b64d2a19ecf153235604e7e3f78b
