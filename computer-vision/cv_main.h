#ifndef __CV_MAIN_H__
#define __CV_MAIN_H__

#include <iostream>
#include "stdint.h"
#include <ctime>

#define MAX_BBOXES 5

// holds the data needed to draw an individual bounding box
struct coordinate_data {
  uint16_t x_coord;
  uint16_t y_coord;
  uint16_t x_len;
  uint16_t y_len;
};

// struct that will be transmitted and contains all necessary CV info
struct cv_data {
  int num_bbox = 0; // number of bounding boxes
  time_t t = time(0); // timestamp (will be provided by camera team)
  struct coordinate_data box_data[MAX_BBOXES]; // collection of bounding boxes
};

#endif
