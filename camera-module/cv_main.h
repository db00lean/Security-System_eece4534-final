#ifndef __CV_MAIN_H__
#define __CV_MAIN_H__
#include "../common_headers/cv_structs.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

// TODO: Rename "cv_main.cpp" and "cv_main.h" later on
// Having a header for functions seems like it could be nice, but is it needed?

cv::Mat ImportFrame();
cv_data GenerateBBoxes(
    cv::Mat image); // Change datatype from int to whatever the frame is
int TransmitStruct(cv_data data_to_send);

#endif
