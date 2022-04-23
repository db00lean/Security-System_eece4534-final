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
    .menuMode = 0,
    .running = 1
};

void stop_threads(int _sig) {
  if (securitySystem.running) {
    securitySystem.running = 0;
    printf("\n[ Main ] - Terminating threads...\n");
    printf("[ Main ] - Press any button on the ZedBoard to terminate the button thread...\n");
  }
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

int initialize_cameras() {
  enumerate_cameras(); 
  
  // dynamically allocate memory for cams depending on how many we have
  securitySystem.cameras = malloc(securitySystem.numberOfCameras * sizeof(camera_module));
  // init each camera
  for (int ii = 0; ii < securitySystem.numberOfCameras; ii++) {
      if(initialize_camera(ii)) {
        return -1;
      }
  }

  return 0; 
} 

int main(int argc, char **argv) {
  int ret;

  pthread_t btn_listener_thread, hdmi_thread;

  // init metadata network
  received_message* msg;
  const char* port = "55000";
  struct server* networkServer = new_server(port);
  
  // initalize
  if (initialize_cameras()) {
    printf("[ Main ] - Camera initialization failed...\n"); 
    return -1;
  }; 
  
  if (initialize_buttons()) {
    printf("[ Main ] - Button initialization failed... Make sure kernel module has been inserted\n");
    return -1; 
  }

  if (initialize_hdmi()) {
    printf("[ Main ] - HDMI initialization failed... Make sure cable has been plugged in\n");
    return -1; 
  }; 

  print_system_info();

  signal(SIGINT, stop_threads);
  // launching threads
  pthread_create(&btn_listener_thread, NULL, run_button_client, &securitySystem);
  pthread_create(&hdmi_thread, NULL, hdmi_main ,&securitySystem);

  // get metadata from the network
  while (securitySystem.running) {
    msg = receive_msg(networkServer->responder);
    if (msg == NULL) {
      printf("[ Main ] - Received NULL msg\n");
      continue;
    }

    securitySystem.cameras[0].cvMetadata = *((struct cv_data*) msg->data);
    printf("Received message\n");
    printf("active camera: %i\n", securitySystem.guiState);
    printf("Camera id: %i\n", msg->cam_id);
    printf("Data type: %i\n", msg->type);
    printf("Data length: %i\n", msg->len);

    // Perform a detection of whether or not a person is in the FZ on camera n
    aggregate_detect(&securitySystem.cameras[0]);
  }

  // cleanup
  pthread_mutex_destroy(&securitySystem.lock);
  pthread_join(btn_listener_thread, NULL);
  pthread_join(hdmi_thread, NULL);

  free(securitySystem.cameras);
  free(networkServer);

  printf("[ Main ] - Security camera system exited successfully. Bye!\n");

  return 0;
}
