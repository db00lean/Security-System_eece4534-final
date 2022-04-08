// definitions commonly used in system management code
// John Craffey

#ifndef SYSTEM_MANAGEMENT_H
#define SYSTEM_MANAGEMENT_H

#include "cv_structs.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define MAX_NUM_CAMERAS 8
#define MAX_NUM_PEOPLE 64

// struct to hold values unique to each individual camera module
typedef struct camera_module {
  int cameraNumber;
  // values for networking
  int sysManPortNumber;
  int streamPortNumber;

  // values important to GUI output of security system
  int status;                           // is the camera on or off
  int detection;                        // is a person detected on the camera
  struct coordinate_data forbiddenZone; // coords of the forbidden zone
  struct cv_data
      cvMetadata; // coordinates of bounding boxes/other info from CV module
} camera_module;

// structure to hold key information of the whole system
typedef struct system_status {
  int numberOfCameras;
  int guiState; // which camera the GUI is supposed to be viewing
  struct camera_module *cameras;
  pthread_mutex_t lock;
} system_status;

#endif
