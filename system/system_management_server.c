// System management server runs on security system base station
// John Craffey

#include <stdlib.h>
#include "system_management.h"

// check if there is a camera that is unitialized
void enumerate_cameras() {
    // TODO make this actually check if a camera is waiting to be initialized
    securitySystem.numberOfCameras = 2;
}

// set important values for each camera module
int initialize_camera(int cameraNumber) {
    // TODO ports need to be calculated via some networking code
    int ports[2] = {123, 456};

    securitySystem.cameras[cameraNumber].sysManPortNumber = ports[cameraNumber];
    securitySystem.cameras[cameraNumber].metaPortNumber = ports[cameraNumber];
    securitySystem.cameras[cameraNumber].streamPortNumber = ports[cameraNumber];

    return 0;
}

int main(int argc, char **argv) {

    // init the cameras
    // find out how many cams
    enumerate_cameras();
    // allocate memory for cams
    securitySystem.cameras = malloc(securitySystem.numberOfCameras * sizeof(camera_module));
    // init each camera
    for (int ii = 0; ii < securitySystem.numberOfCameras; ii++) {
        initialize_camera(ii);
    }

    // get metadata and send to data agregator
    // TODO implement something like metadata = getData(securitySystem.cameras[cameraNumber].metaPortNumber);
    


    // cleanup
    free(securitySystem.cameras);

    return 0;
}
