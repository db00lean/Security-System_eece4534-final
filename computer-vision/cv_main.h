#ifndef __CV_MAIN_H__
#define __CV_MAIN_H__

#include <iostream>
#include "stdint.h"
#include <ctime>

#define MAX_BBOXES 5

struct coordinate_data {
  uint16_t x_coord;
  uint16_t y_coord;
  uint16_t x_len;
  uint16_t y_len;
};

struct cv_data {
  int num_bbox = 0;
  time_t t = time(0);
  struct coordinate_data box_data[MAX_BBOXES];
};

#endif
