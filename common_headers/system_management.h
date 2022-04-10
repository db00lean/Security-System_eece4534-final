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
  struct cv_data cvMetadata; // coordinates of bounding boxes/other info from CV module
} camera_module;

// structure to hold key information of the whole system
typedef struct system_status {
  int numberOfCameras;
  int guiState; // which camera the GUI is supposed to be viewing. 0-indexed, indexes into list of cameras.
  camera_module *cameras;
  uint8_t menuMode; // determines which set of button actions to execute
  pthread_mutex_t lock; // protects this structure.
} system_status;

/**
 * @brief Gets the forbidden zone of the currently active camera. Uses system_status's 
 *        lock for synchronization.
 * 
 * @param system - the system_status struct containing the forbidden zone 
 * @return struct coordinate_data - read-only copy of the forbidden zone
 */
struct coordinate_data get_forbidden_zone(system_status* system);

/**
 * @brief Get the active camera object. Uses system_status's lock for synchronization
 * 
 * @param system - the system_status struct containing all the camera modules
 * @return camera_module* - pointer to the currently active camera
 */
camera_module* get_active_camera(system_status* system);

#endif
