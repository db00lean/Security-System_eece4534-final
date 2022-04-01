# Computer Vision

### Responsibilities
- Retrieve images from camera driver via IPC
- Process images, detect humans using CV and generate coordinates for boxes around them
- Package and forward coordinate data for all bounding boxes to HDMI for transmission to base system

### Componenets
*Image Processing*
- Receives images from the Camera
- Generate bounding boxes around people/objects detected on each camera
- Sends bounding box data to HDMI

### Inputs
- Frames/Images from camera

### Outputs
- Coordinates of bounding boxes of detected people/objects

### Members
John Privitera (leader) - Initial focus on generating bounding boxes

Jack Horton - Initial focus on outputting the bounding box data to HDMI

Nicole Johnson - Initial focus on receiving images from camera's interface

### Usage Examples:

```
export PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig/
g++ test.cpp -o testoutput `pkg-config --cflags --libs opencv`
```

### Dependencies and Installation

----------------------------------------------------------------------------------------------------------------------------------------------------------------------
OpenCV installation:

Step 1:
```
sudo apt-get update && sudo apt-get upgrade
sudo apt install software-properties-common
sudo apt install apt-file
```
Step 2:
```
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-add-repository ppa:deadsnakes/ppa
sudo apt-get install python3.5-dev python3-numpy libtbb2 libtbb-dev
```
Step 3:
```
sudo apt-add-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
sudo apt update
sudo apt install libjasper1 libjasper-dev
sudo apt-get install libjpeg-dev libpng-dev libtiff5-dev libjasper-dev libdc1394-22-dev libeigen3-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev sphinx-common libtbb-dev yasm libfaac-dev libopencore-amrnb-dev libopencore-amrwb-dev libopenexr-dev libgstreamer-plugins-base1.0-dev libavutil-dev libavfilter-dev libavresample-dev
```

Step 4:
```
sudo -s
cd /opt
git clone https://github.com/Itseez/opencv.git
git clone https://github.com/Itseez/opencv_contrib.git
```
Step 5:
```
cd opencv
mkdir release
cd release
cmake -D BUILD_TIFF=ON -D WITH_CUDA=OFF -D ENABLE_AVX=OFF -D WITH_OPENGL=OFF -D WITH_OPENCL=OFF -D WITH_IPP=OFF -D WITH_TBB=ON -D BUILD_TBB=ON -D WITH_EIGEN=OFF -D WITH_V4L=OFF -D WITH_VTK=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D OPENCV_GENERATE_PKGCONFIG=ON -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv/
make -j4
make install
ldconfig
exit
cd ~
```
Step 6:
```
pkg-config --modversion opencv
```
IF PACKAGE NOT FOUND:
```
apt-file search opencv.pc
ls /usr/local/lib/pkgconfig/
sudo cp /usr/local/lib/pkgconfig/opencv4.pc  /usr/lib/x86_64-linux-gnu/pkgconfig/opencv.pc
pkg-config --modversion opencv
```
IF STILL NOT FOUND:
Do:
```
apt-file search opencv.pc
```
and take that path and put it here:
```
export PKG_CONFIG_PATH= PATH
```
path should usually be PATH = /usr/lib/x86_64-linux-gnu/pkgconfig/

IF STILL NOT FOUND:
```
cd /usr/include/
mv opencv4/opencv2/ opencv2
```

----------------------------------------------------------------------------------------------------------------------------------------------------------------------
Steps taken from: http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/#:~:text=Install%20OpenCV%20C%20C%2B%2B%20in%20Ubuntu%2018.04%20LTS,opencv%201%20%24%20pkg-config%20--modversion%20opencv.%20More%20items


