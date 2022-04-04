// Joshua Erickson

#include "aggregate_detect.h"
#include "system_management.h"
#include <stdio.h>

int detect(struct coordinate_data rzone_data, struct coordinate_data box_data) {
    int x1, x2, y1, y2; // Person bounding box values
    int rzx1, rzx2, rzy1, rzy2;     // Restricted zone values
    int contains, inside;
    x1 = box_data.x_coord;
    x2 = box_data.x_coord + box_data.x_len;
    y1 = box_data.y_coord;
    y2 = box_data.y_coord + box_data.y_len;
    rzx1 = rzone_data.x_coord;
    rzx2 = rzone_data.x_coord + rzone_data.x_len;
    rzy1 = rzone_data.y_coord;
    rzy2 = rzone_data.y_coord + rzone_data.y_len;

    inside = (((x1 <= rzx1 && rzx1 <= x2) || (x1 <= rzx2 && rzx2 <= x2)) && ((y1 <= rzy1 && rzy1 <= y2) || (y1 <= rzy2 && rzy2 <= y2)));
    contains = (((rzx1 <= x1 && x1 <= rzx2) || (rzx1 <= x2 && x2 <= rzx2)) && ((rzy1 <= y1 && y1 <= rzy2) || (rzy1 <= y2 && y2 <= rzy2)));

    return inside || contains;
}

// Sets the detection field on the camera module to 1 if someone is within the restricted zone
// Input: cam as the camera module with the suspected person
// Input: bounding box input in system_management.h
void aggregate_detect(struct camera_module cam) {
    // cam.detection = detect(cam.forbiddenZone, cam.cvMetadata.box_data[0]);
    cam.detection = detect(cam.cvMetadata.box_data[1], cam.cvMetadata.box_data[0]);

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
    // printf("Forbidden zone: %d, %d, %d, %d\n", cam.forbiddenZone.x_coord, cam.forbiddenZone.y_coord, cam.forbiddenZone.x_len, cam.forbiddenZone.y_len);
}

// Main for testing purposes
#ifdef DEBUG
// int main(int argc, char **argv) {
//     struct camera_module cam;
//     struct cv_data metadata = {
//         .num_bbox = 2,
//         .t = 1020,
//         .box_data[0].x_coord = 5,
//         .box_data[0].y_coord = 5,
//         .box_data[0].x_len = 10,
//         .box_data[0].y_len = 10,
//         .box_data[1].x_coord = 0,
//         .box_data[1].y_coord = 16,
//         .box_data[1].x_len = 20,
//         .box_data[1].y_len = 10,
//     };
//     cam.cvMetadata = metadata;

//     aggregate_detect(cam);
// }

#endif
