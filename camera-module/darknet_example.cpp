#include <iostream>
#include <darknet.h>

using namespace std;

int main() {
    cout << "Darknet application" << endl;

    // -----------------------------------------------------------------------------------------------------------------
    // Define constants that were used when Darknet network was trained.
    // This is pretty much hardcoded code zone, just to give an idea what is needed.
    // -----------------------------------------------------------------------------------------------------------------

    // Path to configuration file.
    static char *cfg_file = const_cast<char *>("/home/jack/code/darknet/cfg/yolov3-tiny.cfg");
    // Path to weight file.
    static char *weight_file = const_cast<char *>("yolov3-tiny.weights");
    // Path to a file describing classes names.
    static char *names_file = const_cast<char *>("../../darknet/data/coco.names");
    // This is an image to test.
    static char *input = const_cast<char *>("people.jpg");
    // Define thresholds for predicted class.
    float thresh = 0.5;
    float hier_thresh = 0.5;
    // Uncomment this if you need sort predicted result.
//    float nms = 0.45;

    // Number of classes in "obj.names"
    // This is very rude method and in theory there must be much more elegant way.
    size_t classes = 0;
    char **labels = get_labels(names_file);
    while (labels[classes] != nullptr) {
        classes++;
    }
    cout << "Num of Classes " << classes << endl;

    // -----------------------------------------------------------------------------------------------------------------
    // Do actual logic of classes prediction.
    // -----------------------------------------------------------------------------------------------------------------
    cout << "check"<<endl;
    // Load Darknet network itself.
    network *net = load_network(cfg_file, weight_file, 0);
    cout << "check"<<endl;
    // In case of testing (predicting a class), set batch number to 1, exact the way it needs to be set in *.cfg file
    set_batch_network(net, 1);

    // Load test image.
    image im = load_image_color(input, 0, 0);
    // And scale it to the parameters define din *.cfg file.
    image sized = letterbox_image(im, net->w, net->h);
    // Uncomment this if you need sort predicted result.
//    layer l = net->layers[net->n - 1];

    // Get actual data associated with test image.
    cout << "check"<<endl;
    float *frame_data = sized.data;
cout << "check"<<endl;
    // Do prediction.
    double time = what_time_is_it_now();
    network_predict(net, frame_data);
    cout << "'" << input << "' predicted in " << (what_time_is_it_now() - time) << " sec." << endl;

    // Get number fo predicted classes (objects).
    image **alphabet = load_alphabet();
    int num_boxes = 0;
    detection *detections = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, nullptr, 1, &num_boxes);
    cout << "Detected " << num_boxes << " obj, class " << detections->classes << endl;

    // Uncomment this if you need sort predicted result.
//    do_nms_sort(detections, num_boxes, l.classes, nms);

    // -----------------------------------------------------------------------------------------------------------------
    // Print results.
    // -----------------------------------------------------------------------------------------------------------------

    // Iterate over predicted classes and print information.
    for (int8_t i = 0; i < num_boxes; ++i) {
        for (uint8_t j = 0; j < classes; ++j) {
            if (detections[i].prob[j] > thresh) {
                // More information is in each detections[i] item.
                cout << labels[j] << " " << (int16_t) (detections[i].prob[j] * 100) << endl;
            }
        }
    }
    draw_detections(im, detections, num_boxes, thresh, labels, alphabet, detections->classes);
    save_image(im, "predictions");

    // -----------------------------------------------------------------------------------------------------------------
    // Free resources.
    // -----------------------------------------------------------------------------------------------------------------

    free_detections(detections, num_boxes);
    free_image(im);
    free_image(sized);
    free(labels);

    return 0;
}
