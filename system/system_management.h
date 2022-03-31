// definitions commonly used in system management code
// John Craffey

#ifndef SYSTEM_MANAGEMENT_H
#define SYSTEM_MANAGEMENT_H

#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<stdint.h>

#define MAX_NUM_CAMERAS 8
#define MAX_NUM_PEOPLE 64

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

// TODO make this just a declaration and
// joshua is going to implement this in his own file
// void aggregate_detect(struct camera_module cam);

// dump all data for the system
void print_system_info(){
    printf("\nSystem info:\n");
    printf("number of cameras: %d\n", securitySystem.numberOfCameras);
    printf("\n");

    for(int ii = 0; ii < securitySystem.numberOfCameras; ii++){
        printf("camera %d\n", ii);
        printf("sysManPortNumber: %d\n", securitySystem.cameras[ii].sysManPortNumber);
        printf("streamPortNumber: %d\n", securitySystem.cameras[ii].streamPortNumber);

        printf("status: %d\n", securitySystem.cameras[ii].status);
        printf("detection: %d\n", securitySystem.cameras[ii].detection);

        printf("\n");
    }
}

#endif
