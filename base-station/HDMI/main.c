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
#include "system_management.h"
#include "cv_structs.h"

// an include for a header file not created yet, could be inherited from draw_bounding_box function head
// #include "rendering.h"
// - bounding_box_colors[] = {red, orange, yelow, light_green, green, teal, blue, violet, grey};

/**
 * @brief draws the background of the GUI (static elements)
 * 
 */
void show_background() {
    // TODO: draw shape/text functions for static elements of the GUI
}

/**
 * @brief draws the current frame from active camera
 * 
 * @param system Struct holding system information
 */
void show_camera_frame(system_status * system) {
    // get the index of the active camera from the guistate
    int active_camera_no = system->guiState;

    // pass the camera number to get the frame corresponding to the active camera number
    char * frame = get_frame(active_camera_no);

    // draw the image
    draw_image(frame);

    // free the memory space of the frame
    free(frame)
}

/**
 * @brief draws the active camera's bounding boxes
 * 
 * @param system Struct holding system information
 */
void show_bounding_box(system_status * system) {
    // get the current camera information and the metadata of that camera
    camera_module * active_camera = system->cameras[system->guiState];
    cv_data * camera_metadata = active_camera->cvMetadata;

    // loop through each bounding box and draw
    int b;
    for (b = 0; b < camera_metadata->num_bbox; b ++) {
        coordinate_data * box_data = camera_metadata->coordinate_data;
        draw_bounding_box(box_data->x_coord,
                          box_data->y_coord,
                          box_data->x_len,
                          box_data->y_len,
                          bounding_box_colors[b]);
    }
}

/**
 * @brief draws the camera information on the GUI
 * 
 * @param system Struct holding system information
 */
void show_camera_info(system_status * system) {
    // get the current camera information from the struct
    camera_module * active_camera = system->cameras[system->guiState];

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
    coordinate_data * zone_data = active_camera->forbiddenZone;
    draw_bounding_box(zone_data->x_coord,
                      zone_data->y_coord,
                      zone_data->x_len,
                      zone_data->y_len,
                      grey);

    // could draw other information, like number of people here
}

/**
 * @brief draws any GUI options
 * 
 * @param system Struct holding system information
 */
void show_camera_options(system_status * system) {
    // TODO: no toggling options are included in the system struct, current mockup includes them.
    //       There would have to be elemetns added to the system_status struct
}

/**
 * @brief renders the GUI and outputs to HDMI
 * 
 */
void render() {
    // TODO: figure out how to get pointer to system management struct
    system_status * system;

    // draw GUI elements
    show_background();
    show_camera_frame(system);
    show_bounding_box(system);
    show_camera_info(system);
    show_camera_options(system);
}

int main() {
    // run indefinitely
    while (1) {
        render();
    }
}