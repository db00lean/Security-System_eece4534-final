CFLAGS:=-g $(shell pkg-config --cflags gstreamer-1.0 gstreamer-rtsp-server-1.0)
LDFLAGS:= $(shell pkg-config --libs gstreamer-1.0 gstreamer-rtsp-server-1.0)

test-launch: test-launch.o
	$(CC) -o test-launch test-launch.o $(LDFLAGS)


clean:
	rm -rf *.o test-launch