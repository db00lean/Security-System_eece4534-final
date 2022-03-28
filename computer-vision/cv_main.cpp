#include "cv_main.h"
#include <iostream>
#include "file.h"
#include "stdio.h"

// TODO: Rename "cv_main.cpp" and "cv_main.h" later on
// TODO: Update Makefile with the new name
// TODO: Remove <iostream> after cout testing is done

// Arbitrary value for now
#define MAX_IMAGE_SIZE 1000

// TODO: Read from buffer created by Camera team instead of this one
// image buffer to read from
char image_buffer[MAX_IMAGE_SIZE];

// Loads an image into the image buffer
int ImportFrame()
{
  // TODO: Implement ImportFrame()
  std::cout << "Importing frame\n";

  // Frames will be read from a buffer created by the Camera team.
  // Since this hasn't been implemented yet, it is simulated here
  // by opening an image and loading it into a buffer.
  FILE * fp = 0;
  fp = fopen("people.jpg", "rb");

  if(fp ==- NULL)
  {
    printf("Error opening file\n");
    return -1;
  }

  // determine file size
  fseek(fp, 0, SEEK_END);
  long int size = ftell(fp);

  if(size > MAX_IMAGE_SIZE)
  {
    size = MAX_IMAGE_SIZE;
  }

  //load image into a buffer
  fseek(fp, 0, SEEK_SET);
  fread(&image_buffer, sizeof(uint8_t), size, fp);

  // TODO: Add error Handling
  fclose(fp);
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
  int err = 0;
  err = ImportFrame();

  if(err < 0)
  {
    return -1;
  }

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
