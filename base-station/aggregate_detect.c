// Joshua Erickson

#include "aggregate_detect.h"
#include "../common_headers/system_management.h"
#include <stdio.h>

int detect(struct coordinate_data rzone_data, struct coordinate_data box_data) {
  int x1, x2, y1, y2;         // Person bounding box values
  int rzx1, rzx2, rzy1, rzy2; // Restricted zone values
  int contains, inside;
  x1 = box_data.x_coord;
  x2 = box_data.x_coord + box_data.x_len;
  y1 = box_data.y_coord;
  y2 = box_data.y_coord + box_data.y_len;
  rzx1 = rzone_data.x_coord;
  rzx2 = rzone_data.x_coord + rzone_data.x_len;
  rzy1 = rzone_data.y_coord;
  rzy2 = rzone_data.y_coord + rzone_data.y_len;

  inside = (((x1 <= rzx1 && rzx1 <= x2) || (x1 <= rzx2 && rzx2 <= x2)) &&
            ((y1 <= rzy1 && rzy1 <= y2) || (y1 <= rzy2 && rzy2 <= y2)));
  contains = (((rzx1 <= x1 && x1 <= rzx2) || (rzx1 <= x2 && x2 <= rzx2)) &&
              ((rzy1 <= y1 && y1 <= rzy2) || (rzy1 <= y2 && y2 <= rzy2)));

  return inside || contains;
}

// Sets the detection field on the camera module to 1 if someone is within the
// restricted zone 
// Input: cam as the camera module with the suspected person
void aggregate_detect(struct camera_module *cam) {
  int detection = 0;
  int ii;
  for (ii = 0; ii < cam->cvMetadata.num_bbox; ii++) {
    detection |= detect(cam->forbiddenZone, cam->cvMetadata.box_data[ii]);
    printf("\nMetadata for camera %d\n", cam->cameraNumber);
    printf("number of bbox: %d\n", cam->cvMetadata.num_bbox);
    printf("detection box %d: %d\n", ii, detection);
  }
  cam->detection = detection;
}

void area_aggregate_detect(struct system_status *state, int cam_id) {
  int detection = 1;
  aggregate_detect(&state->cameras[cam_id]);

  int ii;
  for (ii = 0; ii < state->numberOfCameras; ii++) {
    // TODO add in check about ID of bounding boxes
    detection &= state->cameras[ii].detection;
  }
  state->detection = detection;
}

// Main for testing purposes
#ifdef DEBUG
// int main(int argc, char **argv) {
//   struct coordinate_data fz = {
//     .x_coord = 10,
//     .y_coord = 10,
//     .x_len = 10,
//     .y_len = 10,
//   };

//   struct cv_data metadata1 = {
//       .num_bbox = 1,
//       // .t = 1020,
//       .box_data[0].x_coord = 5,
//       .box_data[0].y_coord = 5,
//       .box_data[0].x_len = 10,
//       .box_data[0].y_len = 10,
//   };

//   struct cv_data metadata2 = {
//       .num_bbox = 1,
//       .box_data[0].x_coord = 21,
//       .box_data[0].y_coord = 21,
//       .box_data[0].x_len = 20,
//       .box_data[0].y_len = 10,
//   };

//   struct system_status system = {
//     .numberOfCameras = 2,
//   };
//   system.cameras = (camera_module*) malloc(system.numberOfCameras * sizeof(camera_module));
  
//   system.cameras[0].cameraNumber = 0;
//   system.cameras[0].forbiddenZone = fz;
//   system.cameras[0].cvMetadata = metadata1;

//   system.cameras[1].cameraNumber = 1;
//   system.cameras[1].forbiddenZone = fz;
//   system.cameras[1].cvMetadata = metadata2;

//   area_aggregate_detect(&system, 0);
//   area_aggregate_detect(&system, 1);
//   printf("System detection: %d, cam1: %d, cam2:%d\n", system.detection, system.cameras[0].detection, system.cameras[1].detection);
// }

#endif
