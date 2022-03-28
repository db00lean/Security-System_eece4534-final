/*
Temp file because it was late and I need to get the bounding box stuff stub'd
*/
#include <iostream>
#include "cv_main.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/core/mat.hpp>

// TODO: Rename "cv_main.cpp" and "cv_main.h" later on
// TODO: Update Makefile with the new name
// TODO: Remove <iostream> after cout testing is done

// Arbitrary value for now
#define MAX_IMAGE_SIZE 1000

// TODO: Read from buffer created by Camera team instead of this one
// image buffer to read from
char image_buffer[MAX_IMAGE_SIZE];

// Loads an image into the image buffer
// TODO: Convert ImportFrame() to C++
cv::Mat LoadImage()
{
    // TODO: Implement ImportFrame()
    std::cout << "Importing frame\n";

    // Read the image file as
    // imread("default.jpg");
    cv::Mat image = cv::imread("people.jpg", cv::IMREAD_GRAYSCALE);

    // Error Handling
    // TODO: Fix error handling on merge with cv_main
    if (image.empty())
    {
        std::cout << "Invalid Image\n";
    }

    return image;
}

cv_data MakeBBoxes(cv::Mat image)
{
    // https://docs.opencv.org/3.4/d1/de5/classcv_1_1CascadeClassifier.html#a90fe1b7778bed4a27aa8482e1eecc116
    struct cv_data cv_data_output;

    // UPDATE: hog is not really working
    // hog (Histogram of Oriented Gradients) should deal with detection
    // Might swap to boundingRect(thresh) --> Need to look into accuracy/resource usage of both
    cv::HOGDescriptor hog;
    // hog = cv::HOGDescriptor::HOGDescriptor();
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    int err = 0; // For error handling: use later
    std::vector<cv::Rect> bboxes;
    // std::vector<int> num_detections;

    // Uncalibrated, unscaled hog (Histogram of Oriented Gradients) --> Probably too expensive currently; TODO: REDUCE
    // detectMultiScale(image, objects, numDetections, scaleFactor, minNeighbors, flags, minSize, maxSize)
    // detectMultiScale(InputArray, vector<Rect>&, vector<int>&, double, int, int, Size, Size)
    // hog.detectMultiScale(image, bboxes, num_detections, 1.0, 3, 0, cv::Size(12, 12), cv::Size(96, 96));
    hog.detectMultiScale(image, bboxes, 1.0, 3, 0, cv::Size(8, 8), cv::Size(64, 64));

    // TODO: Add error Handling
    if (err < 0)
    {
        std::cout << "Unable to generate bounding boxes\n";
    }
    return cv_data_output;
}

int SendStruct(cv_data data_to_send)
{
    // TODO: Implement TransmitStruct()
    std::cout << "Sending cv_data out\n";

    // TODO: Add error Handling
    return 0;
}

int main()
{
    struct cv_data cv_data_current;
    struct coordinate_data box1; // Not used
    struct coordinate_data box2; // Not used
    cv::Mat image;
    int err = 0;
    image = LoadImage();

    if (image.empty())
    {
        return -1;
    }

    cv_data_current = MakeBBoxes(image);

    // struct coordinate_data box2;

    // box1/box2 are not used currently
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
