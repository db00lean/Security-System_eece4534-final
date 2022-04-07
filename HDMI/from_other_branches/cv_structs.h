/*
I am including this because it is not on the branch I am working on, but this is in the cv branch under project-security-system/computer-vision/cv_structs.h
*/
#ifndef __CV_STRUCT_H__
#define __CV_STRUCT_H__

#include <stdint.h>

#define MAX_B_BOXES 8

// holds the data needed to draw an individual bounding box
struct coordinate_data
{
  uint16_t x_coord;
  uint16_t y_coord;
  uint16_t x_len;
  uint16_t y_len;
};

// struct that will be transmitted and contains all necessary CV info
struct cv_data
{
  int num_bbox;                             // number of bounding boxes
  struct coordinate_data box_data[MAX_B_BOXES]; // collection of bounding boxes
};

#endif