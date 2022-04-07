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
    //top name
    draw_text_scale(50, 50, "Security System", 0xff00ff, 4);
    //camera frame border
    draw_boundingbox(100, 100, 800, 600, 0xffffff);

    //zone status block
    draw_boundingbox(1000, 100, 200, 50, 0xcccccc);
    draw_text_scale(1000, 75, "Zone Status", 0xff00ff, 2);
    draw_text_scale(1025, 112, "Vacant", 0x00ff00, 3);

    //people count block
    draw_boundingbox(1000, 300, 200, 50, 0xcccccc);
    draw_text_scale(1000, 275, "Number of People", 0xff00ff, 2);
    draw_text_scale(1025, 312, "3", 0x00ff00, 3);
    
    //shown box and people status block
    draw_boundingbox(1000, 500, 350, 100, 0xcccccc);
    draw_text_scale(1020, 512, "Show Boxes: Yes", 0x0ff00, 2);
    draw_text_scale(1020, 550, "Show Person #: Yes", 0x00ff00, 2);
    
    //camera status boxes
    draw_boundingbox(100, 750, 150, 150, 0xff0000);
    draw_text_scale(175, 825, "1", 0x00ffff, 10);
    draw_text_scale(100, 750, "O", 0xff0000, 3);

    draw_boundingbox(300, 750, 150, 150, 0xff0000);
    draw_text_scale(375, 825, "2", 0x00ffff, 10);
    draw_text_scale(300, 750, "O", 0xff0000, 3);

    draw_boundingbox(500, 750, 150, 150, 0xff0000);
    draw_text_scale(575, 825, "3", 0x00ffff, 10);
    draw_text_scale(500, 750, "O", 0x00ff00, 3);

/*
    //temp testing of draw image - delete later
    // Using image struct from imagelib.h and fill with data for now
    int IMG_W = 1920;
    int IMG_H = 1080;
    struct image *img = create_image(IMGENC_RGB, IMG_W, IMG_H);
    for (int i = 0; i<IMG_H; i++) {
       for (int j = 0; j<IMG_W; j++) {
           img->buf[i + j + 0] = i % 255;
           img->buf[i + j + 1] = 50;
           img->buf[i + j + 2] = j % 255;
       }   
    }
    //draw image to screen using draw pixel
    // for each box, draw a rectangle bounding box
    for (int x = 0; x < IMG_W; x++) {
        for (int y = 0; y < IMG_H; y++) {
            unsigned int color = (img->buf[x + y] << 16) | (img->buf[x + y + 1] << 8) | (img->buf[x + y + 2] << 0);
            draw_pixel(x,y,color);
        }
    }
*/
    
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