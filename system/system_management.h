// definitions commonly used in system management code
// John Craffey
#ifndef SYSTEM_MANAGEMENT_H
#define SYSTEM_MANAGEMENT_H

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
#endif
