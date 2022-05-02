#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#define DEV_NAME "/dev/video0"

// // TODO: use this to hold some control state to avoid needing to query camera before issuing some controls
// typedef struct control_state_t
// {
//     int hflip;      // boolean
//     int vflip;      // boolean
//     int rotate;     // integer [0, 360]
//     int brightness; // integer (relative)
//     int contrast;   // integer (relative)
//     int pan;        // integer
//     int tilt;       // integer
//     int zoom;       // integer
// } control_state_t;


/**
 * @brief Sets the brightness parameter to val on the webcam specified by dev_name
 * 
 * @param dev_name - path of camera device to adjust brightness of (example: /dev/video0)
 * @param val - brightness value. range: [0, 255] (Note: default brightness is 128)
 * 
 * @return 0 on success, 1 if val is outside range
 */
int set_brightness(char * dev_name, int val);

/**
 * @brief Sets the contrast parameter to val on the webcam specified by dev_name
 * 
 * @param dev_name - path of camera device to adjust contrast of (example: /dev/video0)
 * @param val - contrast value. range: [0, 255] (Note: default contrast is 32)
 * 
 * @return 0 on success, 1 if val is outside range
 */
int set_contrast(char * dev_name, int val);

/**
 * @brief Prints whether a predefined list of controls commands are supported by the device or not. If a control
 *        is available, print the min/max/default values
 * 
 * @param dev_name - path of camera device to print information of (example: /dev/video0)
 * 
 */
void print_controls_info(char * dev_name);

#endif