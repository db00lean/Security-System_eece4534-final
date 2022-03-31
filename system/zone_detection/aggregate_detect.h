// Exposed functions for data aggregation and zone detection
// Joshua Erickson

#include <stdio.h>
#include "../system_management.h"

#ifndef AGGREGATE_DETECT_H
#define AGGREGATE_DETECT_H

// Detects a presence of a person in the restricted area on the given camera as determined through bounding boxes
// Requires access to an updated global structure with bounding box data
void aggregate_detect(struct camera_module cam);

#endif