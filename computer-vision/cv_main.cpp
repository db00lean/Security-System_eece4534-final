#include <iostream>
#include "stdint.h"
#include <ctime>

#define MAX_BBOXES 5

using namespace std;

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

int main() {
  struct cv_data cv_data_current;
  struct coordinate_data box1;
  struct coordinate_data box2;
  // Set box 1
  box1.x_coord = 15; // x coordinate
  box1.y_coord = 100; // y coordinate
  box1.x_len = 20; // x length
  box1.y_len = 50; // y length
  // Set box 2
  box2.x_coord = 15; // x coordinate
  box2.y_coord = 100; // y coordinate
  box2.x_len = 20; // x length
  box2.y_len = 50; // y length
  
  cv_data_current.num_bbox = 2;
  cv_data_current.box_data[0] = box1;
  cv_data_current.box_data[1] = box2;
  
  cout << cv_data_current.box_data[0].x_coord<<endl;
  return 0;
}
