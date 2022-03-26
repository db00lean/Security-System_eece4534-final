#include "cv_main.h"
#include <iostream>
// TODO: Rename "cv_main.cpp" and "cv_main.h" later on
// TODO: Update Makefile with the new name
// TODO: Remove <iostream> after cout testing is done

int ImportFrame()
{
  // TODO: Implement ImportFrame()
  std::cout << "Importing frame\n";

  // TODO: Add error Handling
  return 0;
}

int GenerateBBoxes(int frame)
{
  // TODO: Implement GenerateBBoxes()
  std::cout << "Generate bounding boxes\n";

  // TODO: Add error Handling
  return 0;
}

int TransmitStruct(cv_data data_to_send)
{
  // TODO: Implement TransmitStruct()
  std::cout << "Sending cv_data out\n";

  // TODO: Add error Handling
  return 0;
}

int main()
{
  struct cv_data cv_data_current;
  struct coordinate_data box1;
  struct coordinate_data box2;
  // Set box 1
  box1.x_coord = 15;  // x coordinate
  box1.y_coord = 100; // y coordinate
  box1.x_len = 20;    // x length
  box1.y_len = 50;    // y length
  // Set box 2
  box2.x_coord = 15;  // x coordinate
  box2.y_coord = 100; // y coordinate
  box2.x_len = 20;    // x length
  box2.y_len = 50;    // y length

  cv_data_current.num_bbox = 2;
  cv_data_current.box_data[0] = box1;
  cv_data_current.box_data[1] = box2;

  std::cout << cv_data_current.box_data[0].x_coord << std::endl;
  return 0;
}
