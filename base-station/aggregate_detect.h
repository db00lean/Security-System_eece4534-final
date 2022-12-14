// Exposed functions for data aggregation and zone detection
// Joshua Erickson
#ifndef AGGREGATE_DETECT_H
#define AGGREGATE_DETECT_H

#include "../common_headers/system_management.h"
#include <stdio.h>

// Detects a presence of a person in the restricted area on the given camera as
// determined through bounding boxes Requires access to an updated global
// structure with bounding box data
void aggregate_detect(struct camera_module *cam);

// Detects the presence of a person in the restricted area on a 3d plane
// state: state of system
// cam_id: index of camera that has just changed
void area_aggregate_detect(struct system_status *state, int cam_id);

#endif
