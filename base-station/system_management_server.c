// System management server runs on security system base station
// John Craffey

#include "../common_headers/system_management.h"
#include "../common_headers/button_client.h"
#include "aggregate_detect.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

// global variable to track the system charateristics
system_status securitySystem = {
    .numberOfCameras = 0,
    .menuMode = 0
};

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

  return 0;
}

int main(int argc, char **argv) {
  // Kick off thread for button presses
  pthread_t btn_listener_thread;
  pthread_mutex_init(&securitySystem.lock, 0);
  signal(SIGINT, stop_button_listener);

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

  // get metadata and send to data agregator
  // this is a dummy struct for testing
  struct cv_data metadata = {
      .num_bbox = 2,
      //.t = 1020,
      .box_data[0].x_coord = 5,
      .box_data[0].y_coord = 5,
      .box_data[0].x_len = 10,
      .box_data[0].y_len = 10,
      .box_data[1].x_coord = 5,
      .box_data[1].y_coord = 5,
      .box_data[1].x_len = 10,
      .box_data[1].y_len = 10,
  };
  securitySystem.cameras[0].cvMetadata = metadata;
  // TODO implement something like metadata =
  // getData(securitySystem.cameras[cameraNumber].metaPortNumber);
  // TODO use CV teams data structure for the metadata
  /*
  struct packet_struct packet = receive();
  int type = packet.type;
  if (type == CV_DATA) {
      securitySystem.cameras[0].cvMetadata = packet.payload;
  }
  */

  // Perform a detection of whether or not a person is in the FZ on camera n
  aggregate_detect(securitySystem.cameras[0]);

  // cleanup
  pthread_mutex_destroy(&securitySystem.lock);
  free(securitySystem.cameras);
  pthread_join(btn_listener_thread, NULL);

  return 0;
}
