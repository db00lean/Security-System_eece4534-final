// System management server runs on security system base station
// John Craffey

#include "../common_headers/hdmi_main.h"
#include "../common_headers/system_management.h"
#include "../common_headers/button_client.h"
#include "aggregate_detect.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <czmq.h>
#include "../network/client.h"
#include "../network/server.h"

// global variable to track the system charateristics
system_status securitySystem = {
    .numberOfCameras = 0,
    .menuMode = 0
};

struct coordinate_data get_forbidden_zone(system_status* system) {
  struct coordinate_data fz;
  pthread_mutex_lock(&system->lock);
  fz = system->cameras[system->guiState].forbiddenZone;
  pthread_mutex_unlock(&system->lock);
  return fz; 
}

camera_module* get_active_camera(system_status* system) {
  camera_module* cam;
  pthread_mutex_lock(&system->lock);
  cam = &(system->cameras[system->guiState]);
  pthread_mutex_unlock(&system->lock);
  return cam;
}

// dump all data for the system
void print_system_info() {
  printf("\nSystem info:\n");
  printf("number of cameras: %d\n", securitySystem.numberOfCameras);
  printf("\n");

  for (int ii = 0; ii < securitySystem.numberOfCameras; ii++) {
    printf("camera %d\n", ii);
    printf("sysManPortNumber: %d\n",
           securitySystem.cameras[ii].sysManPortNumber);
    printf("streamPortNumber: %d\n",
           securitySystem.cameras[ii].streamPortNumber);

    printf("status: %d\n", securitySystem.cameras[ii].status);
    printf("detection: %d\n", securitySystem.cameras[ii].detection);

    printf("\n");
  }
}

// check if there is a camera that is unitialized
void enumerate_cameras() {
  // TODO make this actually check if a camera is waiting to be initialized
  securitySystem.numberOfCameras = 2;
}

// set important values for each camera module
int initialize_camera(int cameraNumber) {
  // TODO ports need to be calculated via some networking code
  int ports[2] = {123, 456};

  securitySystem.cameras[cameraNumber].cameraNumber = cameraNumber;
  securitySystem.cameras[cameraNumber].sysManPortNumber = ports[cameraNumber];
  securitySystem.cameras[cameraNumber].streamPortNumber = ports[cameraNumber];

  securitySystem.cameras[cameraNumber].status = 1;

  securitySystem.cameras[cameraNumber].forbiddenZone.x_coord = 0;
  securitySystem.cameras[cameraNumber].forbiddenZone.y_coord = 0;
  securitySystem.cameras[cameraNumber].forbiddenZone.x_len = 150;
  securitySystem.cameras[cameraNumber].forbiddenZone.y_len = 200;



  return 0;
}

// make sure coordinates of bboxes are in range
int validateCVData(struct cv_data* message){
  int isValid = 1;
  int ii;
  for (ii = 0; ii < message->num_bbox; ii++) {
    if (message->box_data[ii].x_coord +  message->box_data[ii].x_len > X_RESOLUTION ||
        message->box_data[ii].y_coord +  message->box_data[ii].y_len > Y_RESOLUTION) {
          isValid = 0;
        }
  }

  return isValid;
}

int main(int argc, char **argv) {
  // for button presse thread
  pthread_t btn_listener_thread;
  pthread_mutex_init(&securitySystem.lock, 0);
  signal(SIGINT, stop_button_listener);
  // for HDMI thread
  pthread_t hdmi_thread;

  // init metadata network
  received_message* msg;
  const char* port = "55000";
  struct server* networkServer = new_server(port);
  // init the cameras
  // find out how many cams
  enumerate_cameras();
  // dynamically allocate memory for cams depending on how many we have
  securitySystem.cameras =
      malloc(securitySystem.numberOfCameras * sizeof(camera_module));
  // init each camera
  for (int ii = 0; ii < securitySystem.numberOfCameras; ii++) {
    initialize_camera(ii);
  }
  // print for debug
  print_system_info();

  // launching button thread
  pthread_create(&btn_listener_thread, NULL, run_button_client, &securitySystem);

  // launching HDMI thread
  pthread_create(&hdmi_thread, NULL, hdmi_main ,&securitySystem);

  // get metadata from the network
  while(1) {
    msg = receive_msg(networkServer->responder);
    // check if the data in the message has valid coordinates
    int valid = validateCVData((struct cv_data*) msg->data);

    if (valid) {
      securitySystem.cameras[msg->cam_id].cvMetadata = *((struct cv_data*) msg->data);
      printf("Received valid message\n");
      printf("active camera: %i\n", securitySystem.guiState);
      printf("Camera id: %i\n", msg->cam_id);
      printf("Data type: %i\n", msg->type);
      printf("Data length: %i\n", msg->len);
  
      // Perform a detection of whether or not a person is in the FZ on camera n
      aggregate_detect(&securitySystem.cameras[msg->cam_id]);
    } else {
      printf("Received INVALID message\n");
      securitySystem.cameras[msg->cam_id].cvMetadata.num_bbox = 0;
    }
  }

  // cleanup
  pthread_mutex_destroy(&securitySystem.lock);
  free(securitySystem.cameras);
  pthread_join(btn_listener_thread, NULL);
  pthread_join(hdmi_thread, NULL);
  free(networkServer);

  return 0;
}
