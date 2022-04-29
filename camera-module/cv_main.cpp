#include "cv_main.h"

#include "../base-station/HDMI/imagelib.h"
#include "../base-station/HDMI/gstreamer-rx.h"

#define NUM_CHANNELS 3
#define ARRAY_DIM 3

#if DEBUG_MODE_CV
#include <iostream>
#endif

// NOTE: Not sure if the current import for darknet.h requires something like dn::do_function()
// or if it could just be referenced directly as do_function()
// It's been hard to find documentation on darknet directly
// Most data is from DarkHelp's Darknet background section and darknet/src/detector.c
// DarkHelp's Darknet background: https://www.ccoderun.ca/programming/2019-08-25_Darknet_C_CPP/

// TODO: Rename "cv_main.cpp" and "cv_main.h" later on
// TODO: Update Makefile with the new name
// TODO: Remove <iostream> and "stdio.h" after cout testing is done

// Loads an image into the image buffer
#if DO_CV
cv::Mat ImportFrame()
{
#if DEBUG_MODE_CV
  // TODO: Implement ImportFrame()
  std::cout << "Importing frame\n";
#endif

  // Read the image file as
  // imread("default.jpg");
  cv::Mat image = cv::imread("people.jpg", cv::IMREAD_GRAYSCALE);

  // Replace cv::imread() with darknet::load_image()
  // Testing with openCV's read first, as darknet's seems to be an opencv wrapper
  // image image = load_image(path, 0, 0, net.c);

  // Error Handling
  // TODO: Fix error handling on merge with cv_main
  if (image.empty())
  {
#if DEBUG_MODE_CV
    std::cout << "Invalid Image\n";
#endif
  }

  return image;
}
#endif


cv::Mat StreamFrame(struct camera_rx* cam)
{
  struct image * img = get_frame(cam, IMGENC_BGR, IMAGE_WIDTH, IMAGE_HEIGHT);

  // convert image to cv Mat
  const int * sizes = {img->height, img->width, NUM_CHANNELS};
  const size_t * steps = {(img->width * sizeof(char)), sizeof(char)};
  cv::Mat cv_frame = cv::Mat::Mat(ARRAY_DIM, sizes, CV_8UC3, (void *) img->buf, steps);
 
  free_image(img);
  return cv_frame;
}

#if DO_CV
cv_data GenerateBBoxes(cv::Mat image)
{
  // https://docs.opencv.org/3.4/d1/de5/classcv_1_1CascadeClassifier.html#a90fe1b7778bed4a27aa8482e1eecc116
  struct cv_data cv_data_output;
  int err = 0; // For error handling: use later
  std::vector<cv::Rect> bboxes;
  std::vector<double> found_weights; // Accuracy of model -- for internal use for the moment

#if USE_DARKNET == 0
  // HOG Descriptors work, but are too slow for the Xilinx board
  // HOG (Histogram of Oriented Gradients) should deal with detection
  // Might swap to boundingRect(thresh) --> Need to look into accuracy/resource usage of both
  // HOG Descriptor code BEGIN
  cv::HOGDescriptor hog;
  hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

  // Uncalibrated, unscaled hog (Histogram of Oriented Gradients) --> Probably too expensive currently; TODO: REDUCE
  hog.detectMultiScale(image, bboxes, found_weights, 0.0, cv::Size(8, 8), cv::Size(16, 16), 1.05, 2.0, false);
/*
detectMultiScale(InputArray img,
                std::vector< Rect > & foundLocations,
                std::vector< double > & 	foundWeights,
                double 	hitThreshold = 0,
                Size 	winStride = Size(),
                Size 	padding = Size(),
                double 	scale = 1.05,
                double 	groupThreshold = 2.0,
                bool 	useMeanshiftGrouping = false
                )		const
*/
// HOG Descriptor code END
#endif

  // TODO: Add error Handling
#if DEBUG_MODE_CV
  if (err < 0)
  {
    std::cout << "Unable to generate bounding boxes\n";
  }
#endif

  int num_box = 0;
  for (auto &element : bboxes)
  {
    // std::cout << "box " << num_box;
    // std::cout << "x = " << element.x << " ";
    // std::cout << "y = " << element.y << " ";
    // std::cout << "width = " << element.width << " ";
    // std::cout << "height = " << element.height << "\n";

    cv_data_output.box_data[num_box].x_coord = element.x;
    cv_data_output.box_data[num_box].y_coord = element.y;
    cv_data_output.box_data[num_box].x_len = element.width;
    cv_data_output.box_data[num_box].y_len = element.height;

    num_box++;

    cv_data_output.num_bbox = num_box;
    if (num_box >= MAX_B_BOXES)
    {
#if DEBUG_MODE_CV
      std::cout << "More people detected than MAX_B_BOXES limit\n";
#endif
      break;
    }
  }

  return cv_data_output;
}
#endif

#if DO_CV
#if USE_DARKNET
// Should make SetupDarknet return a struct that contains setup data
// This struct should contain the loaded cfg info to pass into GenerateBBoxes()
// SHOULD BE TYPE "network" FOR NOW
network SetupDarknet(char *cfgfile, char *weightfile)
{
  // Unsure if the following code will be needed:
  // list *options = read_data_cfg(datacfg);
  // char *name_list = option_find_str(options, "names", "data/names.list");
  // int names_size = 0;
  // char **names = get_labels_custom(name_list, &names_size); // get_labels(name_list);

  // Need to move loading weights to a spot that only executes it once
  // parse_network_cfg_custom()
  // network net = *parse_network_cfg(cfgfile); // set batch=1

  // network *load_network(char *cfg, char *weights, int clear)
  network net = *load_network(cfgfile, weightfile, 0);

  // Need to do load_weights() as well?

  // if (weightfile)
  // {
  //   load_weights(&net, weightfile);
  // }

  // NOTE: The following is passed into test_detector() in darknet/detector.c
  // Should be a good reference for what is needed to actually set up darknet
  // void test_detector(char *datacfg, char *cfgfile, char *weightfile, char *filename, float thresh,
  //    float hier_thresh, int dont_show, int ext_output, int save_labels, char *outfile, int letter_box, int benchmark_layers)
  return net;
}
#endif
#endif

// TODO: Make this accept gstream as an input
// Gets the newest frame from a stream  // TOOO: Make "gstream camera_stream" an input
// Creates/returns coordinates of bounding boxes of people detected in the frame
cv_data GetBBoxesFromFrame()
{
#if DO_CV
  cv::Mat frame;         // Could consolidate this into one mega-line, but this looks cleaner
  frame = ImportFrame(); // TODO: Update ImportFrame() to get frame from gstream; accepts "gstream camera_stream" as argument
  // frame = StreamFrame();
  return GenerateBBoxes(frame);
#else
  // Hard-code 4 bounding boxes
  struct cv_data cv_data_output;
  cv_data_output.num_bbox = 4;
  for (int i = 0; i < 4; i++)
  {
    cv_data_output.box_data[i].x_coord = 240 * i;
    cv_data_output.box_data[i].y_coord = 80 * i;
    cv_data_output.box_data[i].x_len = 60;
    cv_data_output.box_data[i].y_len = 100;
  }
  return cv_data_output;
#endif
}

int cv_main()
{
  struct cv_data cv_data_current;
#if DO_CV
  cv::Mat image;
  image = ImportFrame();

  /**
  struct camera_rx * cam = init_rx_camera("cv_cam");
  image = StreamFrame(cam);
  **/

  if (image.empty())
  {
    return -1;
  }

  cv_data_current = GenerateBBoxes(image);
#else
  cv_data_current = GetBBoxesFromFrame();
#endif

#if DEBUG_MODE_CV
  for (int i = 0; i < cv_data_current.num_bbox; i++)
  {
    std::cout << "box " << i << ": ";
    std::cout << "x = " << cv_data_current.box_data[i].x_coord << "    ";
    std::cout << "y = " << cv_data_current.box_data[i].y_coord << "    ";
    std::cout << "x_len = " << cv_data_current.box_data[i].x_len << "    ";
    std::cout << "y_len = " << cv_data_current.box_data[i].y_len << "\n";
  }
#endif

  return 0;
}
