// definitions commonly used in system management code
// John Craffey

#ifndef SYSTEM_MANAGEMENT_H
#define SYSTEM_MANAGEMENT_H

#include "cv_structs.h"
#include "../base-station/HDMI/inc/gstreamer-rx.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define MAX_NUM_CAMERAS 8
#define MAX_NUM_PEOPLE 64
#define X_RESOLUTION 1920
#define Y_RESOLUTION 1080

#define CAM_BRIGHTNESS_MAX 100
#define CAM_CONTRAST_MAX 100

/**
 * @brief Defines list of menu modes - settings that can be changed via buttons. MODE_LAST designates the last mode
 * 
 */
enum menu_mode {
  MODE_FZ_X = 0, 
  MODE_FZ_Y,
  MODE_FZ_XLEN,
  MODE_FZ_YLEN,
  MODE_CAM_BRIGHTNESS, 
  MODE_CAM_CONTRAST,
  MODE_LAST
};

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
  struct camera_rx* gstream_info;
  uint16_t brightness;
  uint16_t contrast;
} camera_module;

// structure to hold key information of the whole system
typedef struct system_status {
  char running;
  int numberOfCameras;
  int guiState; // which camera the GUI is supposed to be viewing. 0-indexed, indexes into list of cameras.
  camera_module *cameras;
  enum menu_mode mode; // determines which set of button actions to execute
  pthread_mutex_t lock; // protects this structure.
  int detection; // Signifies if the entire system detects a person
} system_status;

/**
 * @brief Stops execution of the button and hdmi threads in response to SIGINT (or any other signal, really). 
 *        Make sure to pass this to the signal syscall (see man 2 signal)
 *        Otherwise, threads will not clean up properly!
 * 
 * @param _sig - Signal (as an integer). Unused
 */
void stop_threads(int _sig);

#endif
