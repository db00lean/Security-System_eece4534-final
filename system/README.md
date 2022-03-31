# System Management

## High level functions to the system as a whole

- self discovery of camera modules over the network
- assert network config info so camera and CV TX/RX know where to send/get data
- provide camera module status back to base station
- provide "user configurable" things such as location of the forbidden zone

## Diagram

![Top level diagram](../media/top_level_diagram.jpg)

## Relevant Structures Used in IPC

```
// TODO DELETE STRUCT ONCE MERGES WITH CV AND HAVE ACCESS TO THEIR HEADER FILE
// holds the data needed to draw an individual bounding box
#define MAX_BBOXES 5
struct coordinate_data {
  uint16_t x_coord;
  uint16_t y_coord;
  uint16_t x_len;
  uint16_t y_len;
};
// TODO DELETE STRUCT ONCE MERGES WITH CV AND HAVE ACCESS TO THEIR HEADER FILE
// struct that will be transmitted and contains all necessary CV info
struct cv_data {
  int num_bbox; // number of bounding boxes
  time_t t; // timestamp (will be provided by camera team)
  struct coordinate_data box_data[MAX_BBOXES]; // collection of bounding boxes
};


// struct to hold values unique to each individual camera module
struct camera_module {
    int cameraNumber;
    // values for networking
    int sysManPortNumber;
    int streamPortNumber;

    // values important to GUI output of security system
    int status; // is the camera on or off
    int detection; // is a person detected on the camera
    struct coordinate_data forbiddenZone; // coords of the forbidden zone
    struct cv_data cvMetadata; // coordinates of bounding boxes/other info from CV module
} camera_module;

// structure to hold key information of the whole system
struct system_status {
    int numberOfCameras;
    int guiState; // which camera the GUI is supposed to be viewing
    struct camera_module *cameras;
} system_status;

// global variable to track the system charateristics
struct system_status securitySystem = {
    .numberOfCameras = 0,
};
```

## Submodule definitions
crossover to other teams in **bold**

- system management client
  - input identification info for discovery via TX/RX module
  - output identification info for discovery via TX/RX module
  - output network information to **stream TX** and to **metadata TX** (port number, etc)
  - output status info back to TX module
- client side TX/RX
  - input data from base station over network
  - output data from base station to system management client 
  - input data from system management client
  - output data from system management client to base station over network
- base station TX/RX
  - input data from client over network
  - output data from client to system management server
  - input data from system management server
  - output data from system management server to client over network
- system management server
  - input data from base station TX/RX
  - output network information to **stream RX** and to **metadata RX** (port number, etc)
  - output configuration info to data aggregator (coordinate of the forbidden zone, later we will determine how the user will set this, hard code for early integration)
- data aggregator/zone detection
  - input metadata from **metadata RX**
  - input configuration info from system management server
  - output decision of alert status to **rendering**

## Members
John Craffey (leader)

Sid Chenrayan

Joshua Erickson

Samir Elhelw
