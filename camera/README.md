# Camera

The Camera subsystem is responsible for capturing images / streaming video from a physical camera and providing
the data captured by the camera to the other subsystems that need them. The code for this subsystem will run on
a zedboard referred to as the "Camera System". There will be at least 2 zedboards running identical code, and perhaps more.
It is yet to be determined if the code developed for the subsystem will be a user-space process or a kernel driver, or some
combination. This will depend on what examples/libraries are available to handle the various responsibilities of the subsystem.

![Top level diagram](../media/top_level_diagram.jpg)

## Inputs

### USB Camera

The subsystem must connect and communicate with a camera, with the goal of receiving a stream of video data. So far,
we know that the camera will be connected via USB to the zedboard, which must act as a USB host to communicate with the
camera. The camera likely acts as a USB Video Class (UVC) device and may already be easily accessible using generic main-line
kernel drivers, once the zedboard is placed into USB host mode.

## Outputs

### Sending video stream data to base station

The subsystem must communicate with the base station in order to send the video stream. The current plan is to select and
use an RTSP library to send the stream over ethernet. This responsbility will require working closely with the system
management and HDMI teams to ensure compatibility with their subsystems.

### Providing video/image data to the Computer Vision subsystem

The computer vision subsystem will primarily or fully run on the same zedboard as the camera subsystem, and will use
the video/image data we provide to detect and relay the positions of people. Unlike sending a constant stream of video
to the base station, we will instead expose an interface to the CV process to request the latest data whenever it needs to.
If the camera subsystem is implemented as a kernel driver, this will likely be a virtual file system interface. Otherwise,
it will be implemented as some other inter-process communication, yet to be determined.

## Team Members

Given that there is one input and two outputs for this subsystem, and three members in our team, it seems to make
the most sense for each of us to focus on one interface.

Ben Palmer is the team leader and will focus on sending the video stream to the base station, as this will probably
require the most coordination with other teams.

Emma Brodigan will focus on connecting to the camera and receiving a stream of video from the camera.

Muhammad Elarbi will focus on providing the video/image data to the computer vision subsystem. At the moment, this
task likely has too many unknowns to make progress, so he will also help with the other tasks / research to understand and
document the entire pipeline of the video stream.

