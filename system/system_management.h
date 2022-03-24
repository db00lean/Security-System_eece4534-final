// definitions commonly used in system management code
// John Craffey

#ifndef SYSTEM_MANAGEMENT_H
#define SYSTEM_MANAGEMENT_H

#include<stdio.h>
#define MAX_NUM_CAMERAS 8
#define MAX_NUM_PEOPLE 64

struct camera_module {
    // values important to camera modules
    int sysManPortNumber;
    int metaPortNumber;
    int streamPortNumber;

    // values important to output of security system
    int status; // is the camera on or off
    int detection; // is a person detected on the camera
    int personCoordinates[MAX_NUM_PEOPLE]; // coordinates of the people

} camera_module;

// structure to hold key information of the whole system
struct system_status {
    int numberOfCameras;
    struct camera_module *cameras;
} system_status;

// global variable to track the system charateristics
struct system_status securitySystem = {
    .numberOfCameras = 0,
};

// dump all data for the system
void barf(){
    printf("\nSystem info:\n");
    printf("number of cameras: %d\n", securitySystem.numberOfCameras);
    printf("\n");

    for(int ii = 0; ii < securitySystem.numberOfCameras; ii++){
        printf("camera %d\n", ii);
        printf("sysManPortNumber: %d\n", securitySystem.cameras[ii].sysManPortNumber);
        printf("metaPortNumber: %d\n", securitySystem.cameras[ii].metaPortNumber);
        printf("streamPortNumber: %d\n", securitySystem.cameras[ii].streamPortNumber);

        printf("status: %d\n", securitySystem.cameras[ii].status);
        printf("detection: %d\n", securitySystem.cameras[ii].detection);
        printf("not printing coordinates till we determine the structure: \n");

        printf("\n");
    }
}

#endif
