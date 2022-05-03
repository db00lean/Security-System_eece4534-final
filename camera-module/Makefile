libs:= -lpthread -lzmq -lczmq $(shell pkg-config --libs gstreamer-1.0 gstreamer-rtsp-server-1.0 opencv4 gstreamer-app-1.0 gstreamer-plugins-base-1.0) -lgstvideo-1.0
CFLAGS:=-g $(shell pkg-config --cflags gstreamer-1.0 gstreamer-rtsp-server-1.0 opencv4) -O

main: system_management_client.o stream.o cv_main.o
	(cd ../network && make cplus)
	(cd ../camera/camera_control && make)
	(cd ../base-station/HDMI && make hdmi-lib.a)
	${CXX} -o main system_management_client.o stream.o cv_main.o ../network/client.o ../network/packet.o ../camera/camera_control/camera_control.o ../base-station/HDMI/hdmi-lib.a ${LDFLAGS} ${libs}

system_management_client.o: system_management_client.cpp
	${CXX} -c system_management_client.cpp ${CFLAGS}

stream.o: stream.cpp
	${CXX} -c stream.cpp ${CFLAGS}

cv_main.o: cv_main.cpp
	${CXX} -c cv_main.cpp ${CFLAGS}

clean:
	rm -f main *.o

