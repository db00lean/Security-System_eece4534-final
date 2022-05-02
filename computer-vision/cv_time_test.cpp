#include <iostream>
#include "../camera-module/cv_main.h"
#include <chrono>

// 4.29856e+06ms for 100 executions of openCV on the Xilinx --> ~43s per frame

// TODO: Rename "cv_main.cpp" and "cv_main.h" later on
// TODO: Update Makefile with the new name
// TODO: Remove <iostream> and "stdio.h" after cout testing is done

// Loads an image into the image buffer
// TODO: Convert ImportFrame() to C++
cv::Mat ImportFrame()
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

cv_data GenerateBBoxes(cv::Mat image)
{
    // https://docs.opencv.org/3.4/d1/de5/classcv_1_1CascadeClassifier.html#a90fe1b7778bed4a27aa8482e1eecc116
    struct cv_data cv_data_output;

    // UPDATE: hog is not really working
    // hog (Histogram of Oriented Gradients) should deal with detection
    // Might swap to boundingRect(thresh) --> Need to look into accuracy/resource usage of both
    cv::HOGDescriptor hog;
    // hog = cv::HOGDescriptor::HOGDescriptor();
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    // int err = 0; // For error handling: use later
    std::vector<cv::Rect> bboxes;
    std::vector<double> found_weights; // Accuracy of model -- for internal use for the moment

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

    // TODO: Add error Handling
    // if (err < 0)
    // {
    //   std::cout << "Unable to generate bounding boxes\n";
    // }

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
            std::cout << "More people detected than MAX_B_BOXES limit\n";
            break;
        }
    }

    return cv_data_output;
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
    cv::Mat image;
    image = ImportFrame();

    if (image.empty())
    {
        return -1;
    }

    //
    auto time_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; i++)
    {
        cv_data_current = GenerateBBoxes(image);
    }
    auto time_end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < cv_data_current.num_bbox; i++)
    {
        std::cout << "box " << i << ": ";
        std::cout << "x = " << cv_data_current.box_data[i].x_coord << "    ";
        std::cout << "y = " << cv_data_current.box_data[i].y_coord << "    ";
        std::cout << "x_len = " << cv_data_current.box_data[i].x_len << "    ";
        std::cout << "y_len = " << cv_data_current.box_data[i].y_len << "\n";
    }

    std::chrono::duration<double, std::milli> ms_double = time_end - time_start;
    std::cout << "Execution time: " << ms_double.count() << "ms\n";

    return 0;
}
