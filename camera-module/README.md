# Camera Module

This code is to be run on the camera module of the security system. It includes a main system management application that launches threads wchich are responsible for the physical camera and RTSP stream, as well at the computer vision/person detection. The system management application is responsible for communication of metadata and CV information (bounding box locations) back to the base station.

## build depencdencies

- czmq ```dnf install libczmq-dev`` on zedboard
- opencv4
- gstreamer (see camera directory for install instructions)

## Build instructions

cross compilation: using the extended sdk and the CZMQ installation found in the network folder readme, enter the camera-module directory and run ```make```

Native compilation: with all of the -dev packages installed, enter the camera-module dir and run ```make```
