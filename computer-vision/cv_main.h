#ifndef __CV_MAIN_H__
#define __CV_MAIN_H__
#include "cv_structs.h"

// TODO: Rename "cv_main.cpp" and "cv_main.h" later on
// Having a header for functions seems like it could be nice, but is it needed?

int ImportFrame();
int GenerateBBoxes(int frame); // Change datatype from int to whatever the frame is
int TransmitStruct(cv_data data_to_send);

#endif