#ifndef __CV_MAIN_H__
#define __CV_MAIN_H__

#include "../common_headers/cv_structs.h"

#include <unistd.h>

extern "C" {
#include "../base-station/HDMI/inc/gstreamer-rx.h"
}

#define DO_CV 1 // Set to 0 to disable all CV and use hard coded boxes

// If USE_DARKNET is 1, cv_main uses darknet. If its 0, it uses openCV's HOG Descriptors
#define USE_DARKNET 0   // Enable Darknet
#define DEBUG_MODE_CV 0 // iostream stuff; can be removed at the end

#if DO_CV
#if USE_DARKNET
#include <darknet.h> // The main "darknet.h" file. Should update this to work with makefile
// #include <darknet/parser.h>
// #include <darknet.h/include/parser.h>
#endif
#endif

#if USE_DARKNET == 0
// Not sure if any opencv needs a direct import besides opencv2/core/mat.hpp and maybe opencv2/opencv.hpp?
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#endif

#if DO_CV
cv::Mat ImportFrame();
cv_data GenerateBBoxes(cv::Mat image); // Change datatype from int to whatever the frame is
#endif
cv_data GetBBoxesFromFrame(struct camera_rx* cam);

// Need to add a struct that contains the darknet setup data
// This would store stuff like the loaded config files to pass into GenerateBBoxes()
//          in addition to a frame/an image

#endif
