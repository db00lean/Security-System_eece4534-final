#ifndef __CV_MAIN_H__
#define __CV_MAIN_H__

#include "../common_headers/cv_structs.h"
#include <darknet.h> // The main "darknet.h" file. Should update this to work with makefile
// #include <darknet/parser.h>
// #include <darknet.h/include/parser.h>

// Not sure if any opencv needs a direct import besides opencv2/core/mat.hpp and maybe opencv2/opencv.hpp?
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

cv::Mat ImportFrame();
cv_data GenerateBBoxes(cv::Mat image); // Change datatype from int to whatever the frame is
cv_data GetBBoxesFromFrame();

// Need to add a struct that contains the darknet setup data
// This would store stuff like the loaded config files to pass into GenerateBBoxes()
//          in addition to a frame/an image

#endif
