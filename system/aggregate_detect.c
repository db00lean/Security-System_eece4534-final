#include "aggregate_detect.h"
#include "system_management.h"
#include <stdio.h>

int detect(struct camera_module cam) {
    return 1;
}

// Sets the detection field on the camera module to 1 if someone is within the restricted zone
// Input: cam as the camera module with the suspected person
// Input: bounding box input in system_management.h
void aggregate_detect(struct camera_module cam) {
    cam.detection = detect(cam);

    printf("\nMetadata for camera %d\n", cam.cameraNumber);
    printf("number of bbox: %d\n",cam.cvMetadata.num_bbox);
    printf("timestamp: %ld\n",cam.cvMetadata.t);
    printf("box 0 xcoord: %d\n", cam.cvMetadata.box_data[0].x_coord);
    printf("box 0 ycoord: %d\n", cam.cvMetadata.box_data[0].y_coord);
    printf("box 0 x len: %d\n", cam.cvMetadata.box_data[0].x_len);
    printf("box 0 y len: %d\n", cam.cvMetadata.box_data[0].y_len);
    printf("box 1 xcoord: %d\n", cam.cvMetadata.box_data[1].x_coord);
    printf("box 1 y coord: %d\n", cam.cvMetadata.box_data[1].y_coord);
    printf("box 1 x len: %d\n", cam.cvMetadata.box_data[1].x_len);
    printf("box 1 y len: %d\n", cam.cvMetadata.box_data[1].y_len);
    printf("detection: %d\n", cam.detection);
}

// Main for testing purposes
int main(int argc, char **argv) {
    struct camera_module cam;
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
    cam.cvMetadata = metadata;

    aggregate_detect(cam);
}
