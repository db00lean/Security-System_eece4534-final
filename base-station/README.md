# Base Station

This code is run on the base station of the security system. It consists of a main system management application that is responsible for kicking off a seperate thread for the HDMI/GUI and one for the button monitor. The management application receives messages from the camera modules containing bounding box information and aggregates it with the forbidden zone information set by the buttons on the zedboard using a custom linux kernel module.

## Build dependancies

- czmq ```dnf install libczmq-dev`` on zedboard
- gstreamer

## Build instructions

cross compilation: using the extended sdk and the CZMQ installation found in the network folder readme, enter the base-station directory and run ```make```

Native compilation: with all of the -dev packages installed, enter the base-station dir and run ```make```

