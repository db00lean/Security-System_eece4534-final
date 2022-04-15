libs:= -lpthread -lzmq -lczmq
CFLAGS:=-g $(shell pkg-config --cflags gstreamer-1.0 gstreamer-rtsp-server-1.0)
LDFLAGS:= $(shell pkg-config --libs gstreamer-1.0 gstreamer-rtsp-server-1.0)

main: system_management_client.o stream.o
	(cd ../network && make cplus)
	${CXX} -o main system_management_client.o stream.o ../network/client.o ../network/packet.o  ${LDFLAGS} ${libs}

system_management_client.o: system_management_client.cpp
	${CXX} -c system_management_client.cpp

stream.o: stream.cpp
	${CXX} -c stream.cpp ${CFLAGS}

clean:
	rm -f main *.o

