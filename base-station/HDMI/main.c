/*
 * main.c - holds skeleton functions for integration between SysMan and HDMI teams
 * 
 * David Boullie 2022
 * 
 * This file offers a blueprint for the integration between the system management team and the HDMI
 * team, with functions to be instantiated in a separate thread to read the sysman struct and to
 * render the GUI image for the HDMI output.
 * 
 */

// includes are placeholders, header files are currently located in separate branches
#include "../common_headers/system_management.h"
#include "../common_headers/cv_structs.h"
#include "inc/draw_bounding_box.h"
#include "inc/drawtext.h"
#include "rtsp-rx/imagelib.h"
#include "inc/DRM_user.h"


// an include for a header file not created yet, could be inherited from draw_bounding_box function head
// #include "rendering.h"
// - bounding_box_colors[] = {red, orange, yelow, light_green, green, teal, blue, violet, grey};

/**
 * @brief draws the background of the GUI (static elements)
 * 
 */
void show_background() {
    //camera status boxes
    draw_text(100, 100, "test123success", 0xffffff);
    draw_boundingbox(100, 300, 50, 50, 0x123123);
    
}

/**
 * @brief draws the current frame from active camera
 * 
 * @param system Struct holding system information
 */
void show_camera_frame(struct system_status * system) {
    // // get the index of the active camera from the guistate
    // int active_camera_no = system->guiState;

    // // pass the camera number to get the frame corresponding to the active camera number
    // char * frame = get_frame(active_camera_no);

    // // draw the image
    // draw_image(frame);

    // // free the memory space of the frame
    // free(frame);
}

/**
 * @brief draws the active camera's bounding boxes
 * 
 * @param system Struct holding system information
 */
void show_bounding_box(struct system_status * system) {
    // get the current camera information and the metadata of that camera
    struct camera_module * active_camera = &system->cameras[system->guiState];
    struct cv_data * camera_metadata = &active_camera->cvMetadata;

    // loop through each bounding box and draw
    int b;
    for (b = 0; b < camera_metadata->num_bbox; b ++) {
        struct coordinate_data * box_data = &camera_metadata->box_data[b];
        draw_boundingbox(box_data->x_coord,
                          box_data->y_coord,
                          box_data->x_len,
                          box_data->y_len,
                          0xffffff //bounding_box_colors[b]
                          );
    }
}

/**
 * @brief draws the camera information on the GUI
 * 
 * @param system Struct holding system information
 */
void show_camera_info(struct system_status * system) {
    // get the current camera information from the struct
    struct camera_module * active_camera = &system->cameras[system->guiState];

    if (active_camera->detection) {
        // draw "ZONE OCCUPIED"
    } else {
        // draw "ZONE VACANT"
    }

    if (active_camera->status) {
        // draw "on"
    } else {
        // draw "off"
    }

    // draw the forbidden zone as an overlay on the camera
    // TODO: coordinate translation between camera coordinates and HDMI coordinates
    struct coordinate_data * zone_data = &active_camera->forbiddenZone;
    draw_boundingbox(zone_data->x_coord,
                      zone_data->y_coord,
                      zone_data->x_len,
                      zone_data->y_len,
                      0xcccccc);

    // could draw other information, like number of people here
}

/**
 * @brief draws any GUI options
 * 
 * @param system Struct holding system information
 */
void show_camera_options(struct system_status * system) {
    // TODO: no toggling options are included in the system struct, current mockup includes them.
    //       There would have to be elemetns added to the system_status struct
}

/**
 * @brief renders the GUI and outputs to HDMI
 * 
 */
void render(struct system_status * system) {
    // TODO: figure out how to get pointer to system management struct
    //init DRM
    int fd, ret;
    fd = drm_open();
    drm_init(fd);
    map = drm_map(fd);

    print_info();
    while (1) {

        // draw GUI elements
        show_background();
      //  show_camera_frame(system);
      //  show_bounding_box(system);
      //  show_camera_info(system);
      //  show_camera_options(system);
    }
}

int main() {
    // run indefinitely
    struct system_status * system;

    render(system);
    
}