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

    securitySystem.cameras[cameraNumber].cameraNumber = cameraNumber;
    securitySystem.cameras[cameraNumber].sysManPortNumber = ports[cameraNumber];
    securitySystem.cameras[cameraNumber].streamPortNumber = ports[cameraNumber];

    securitySystem.cameras[cameraNumber].status = 1;

    return 0;
}

int main(int argc, char **argv) {

    // init the cameras
    // find out how many cams
    enumerate_cameras();
    // dynamically allocate memory for cams depending on how many we have
    securitySystem.cameras = malloc(securitySystem.numberOfCameras * sizeof(camera_module));
    // init each camera
    for (int ii = 0; ii < securitySystem.numberOfCameras; ii++) {
        initialize_camera(ii);
    }
    // print for debug
    print_system_info();

    // get metadata and send to data agregator
    // this is a dummy struct for testing
    struct cv_data metadata = {
        .num_bbox = 2,
        .t = 1020,
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
    // TODO implement something like metadata = getData(securitySystem.cameras[cameraNumber].metaPortNumber);
    // TODO use CV teams data structure for the metadata
    /*
    struct packet_struct packet = receive();
    int type = packet.type;
    if (type == CV_DATA) {
        securitySystem.cameras[0].cvMetadata = packet.payload;
    }
    */

    // TODO talk to joshua about what the interface would be for something like 
    aggregate_detect(securitySystem.cameras[0]);

    // cleanup
    free(securitySystem.cameras);

    return 0;
}
