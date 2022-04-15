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
#include "../../common_headers/hdmi_main.h"
#include "../../common_headers/system_management.h"
#include "../common_headers/cv_structs.h"
#include "inc/draw_bounding_box.h"
#include "inc/drawtext.h"
#include "inc/imagelib.h"
#include "inc/DRM_user.h"



// an include for a header file not created yet, could be inherited from draw_bounding_box function head
// #include "rendering.h"
enum bounding_box_colors{black = 0x000000, white = 0xffffff, red = 0xff0000, orange = 0xffa500, yellow = 0xffff00, light_green = 0xaaff00, green = 0x00ff00, cyan = 0x00ffff, blue = 0x0000ff, violet = 0xff00ff, grey = 0xaaaaaa};
#define MONITOR_H 1080
#define MONITOR_W 1920
#define CAM_SEL_BOX_DIM 150
#define CAM_SEL_BOX_X 100
#define CAM_SEL_BOX_Y 750
#define CAM_SEL_BOX_RAD 10

//general locations of objects to draw
#define IMAGE_TOP_LEFT_X 100
#define IMAGE_TOP_LEFT_Y 100
#define IMAGE_WIDTH MONITOR_W/2
#define IMAGE_HEIGHT MONITOR_H/2
#define TITLE_X MONITOR_W/2
#define TITLE_Y 50
#define ZONE_STATUS_TOP_LEFT_X MONITOR_W/2 + 200
#define ZONE_STATUS_TOP_LEFT_Y IMAGE_TOP_LEFT_Y
#define RIGHT_BOX_H 200
#define RIGHT_BOX_W 400
#define PEOPLE_BOX_TOP_LEFT_X ZONE_STATUS_TOP_LEFT_X
#define PEOPLE_BOX_TOP_LEFT_Y ZONE_STATUS_TOP_LEFT_Y + 300
#define TOGGLE_OPT_BOX_TOP_LEFT_X ZONE_STATUS_TOP_LEFT_X
#define TOGGLE_OPT_BOX_TOP_LEFT_Y PEOPLE_BOX_TOP_LEFT_Y + 300

/**
 * @brief draws the background of the GUI (static elements)
 * 
 */
void show_background() {
    draw_rectangle_filled(0,0,MONITOR_W,MONITOR_H, black);
    //top name
    draw_text_scale(TITLE_X, TITLE_Y, "Security System", light_green, 4);
    //camera frame border
    draw_boundingbox(IMAGE_TOP_LEFT_X, IMAGE_TOP_LEFT_Y, IMAGE_WIDTH, IMAGE_HEIGHT, white);
    
    //shown box and people status block
    draw_boundingbox(TOGGLE_OPT_BOX_TOP_LEFT_X, TOGGLE_OPT_BOX_TOP_LEFT_Y, RIGHT_BOX_W, RIGHT_BOX_H, grey);
    draw_text_scale(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, TOGGLE_OPT_BOX_TOP_LEFT_Y + 50, "Show Boxes: ", light_green, 2);
    draw_circle_filled(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2 + 100, TOGGLE_OPT_BOX_TOP_LEFT_Y + 50, CAM_SEL_BOX_RAD, green);
    draw_text_scale(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, TOGGLE_OPT_BOX_TOP_LEFT_Y + 150, "Show Person #: ", light_green, 2);
    draw_circle_filled(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2 + 100, TOGGLE_OPT_BOX_TOP_LEFT_Y + 150, CAM_SEL_BOX_RAD, green);

    
    //camera status boxes
    // draw_boundingbox(100, 750, 150, 150, 0xff0000);
    // draw_text_scale(175, 825, "1", 0x00ffff, 10);
    // draw_circle_filled(100, 750, 10, 0xff0000);


    // draw_boundingbox(300, 750, 150, 150, 0xff0000);
    // draw_text_scale(375, 825, "2", 0x00ffff, 10);
    // draw_circle_filled(300, 750, 10, 0xff0000);


    // draw_boundingbox(500, 750, 150, 150, 0xff0000);
    // draw_text_scale(575, 825, "3", 0x00ffff, 10);
    // draw_circle_filled(500, 750, 10, 0x00ff00);



    // //temp testing of draw image - delete later
    // // Using image struct from imagelib.h and fill with data for now
    // int IMG_W = 1920;
    // int IMG_H = 1080;
    // struct image *img = create_image(IMGENC_RGB, IMG_W, IMG_H);
    // for (int i = 0; i<IMG_H; i++) {
    //    for (int j = 0; j<IMG_W; j++) {
    //        img->buf[i + j + 0] = i % 255;
    //        img->buf[i + j + 1] = 50;
    //        img->buf[i + j + 2] = j % 255;
    //    }   
    // }
    // // draw image to screen using draw pixel
    // for each box, draw a rectangle bounding box
    // for (int x = TOP_LINE; x < (TOP_LINE + MONITOR_W / 2); x++) {
    //     for (int y = 100; y < (TOP_LINE + MONITOR_H / 2); y++) {
    //         unsigned int color = (img->buf[x + y] << 16) | (img->buf[x + y + 1] << 8) | (img->buf[x + y + 2] << 0);
    //         draw_pixel(x,y,color);
    //     }
    // }

    // free_image(img);   
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

    //people count block
    draw_boundingbox(PEOPLE_BOX_TOP_LEFT_X, PEOPLE_BOX_TOP_LEFT_Y, RIGHT_BOX_W, RIGHT_BOX_H, grey);
    draw_text_scale(PEOPLE_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, PEOPLE_BOX_TOP_LEFT_Y + 50, "Number of People", violet, 2);
    int camera_num_display = camera_metadata->num_bbox + 48;
    draw_text_scale(PEOPLE_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, PEOPLE_BOX_TOP_LEFT_Y + 100, (char *)&camera_num_display, 0x00ff00, 3);

    // loop through each bounding box and draw
    int b;
    for (b = 0; b < camera_metadata->num_bbox; b ++) {
        struct coordinate_data * box_data = &camera_metadata->box_data[b];
        draw_boundingbox(box_data->x_coord,
                          box_data->y_coord,
                          box_data->x_len,
                          box_data->y_len,
                          red
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
    //zone status block
    draw_boundingbox(ZONE_STATUS_TOP_LEFT_X, ZONE_STATUS_TOP_LEFT_Y, RIGHT_BOX_W, RIGHT_BOX_H, grey);
    draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 50, "Zone Status", violet, 2);
    if (active_camera->detection) {
        // draw "ZONE OCCUPIED"
        draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Occupied", red, 3);
    } else {
        // draw "ZONE VACANT"
        draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Vacant", green, 3);
    }

    char num_str[2];
    int status_color;

    for (int i = 0; i < system->numberOfCameras; i++) {

        if (i == system->guiState) {
            draw_boundingbox(CAM_SEL_BOX_X + (i * 200) - 3,
                             CAM_SEL_BOX_Y - 3,
                             CAM_SEL_BOX_DIM + 6,
                             CAM_SEL_BOX_DIM + 6,
                             red);
        }

        draw_rectangle_filled(CAM_SEL_BOX_X + (i * 200),
                              CAM_SEL_BOX_Y,
                              CAM_SEL_BOX_DIM,
                              CAM_SEL_BOX_DIM,
                              grey);

        sprintf(num_str, "%d", i+1);
        draw_text_scale(CAM_SEL_BOX_X + (i * 200) + (CAM_SEL_BOX_DIM / 2),
                        CAM_SEL_BOX_Y + (CAM_SEL_BOX_DIM / 2),
                        num_str,
                        black, 10);

        if (system->cameras[i].status) {
            status_color = green;
        } else {
            status_color = red;
        }

        draw_circle_filled(CAM_SEL_BOX_X + (i * 200) + (CAM_SEL_BOX_RAD / 2),
                           CAM_SEL_BOX_Y + (CAM_SEL_BOX_RAD / 2),
                           CAM_SEL_BOX_RAD,
                           status_color);
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
        show_bounding_box(system);
        show_camera_info(system);
        show_camera_options(system);
        // draw_rectangle_filled(0,0,MONITOR_W,MONITOR_H, black);
    }
}

void* hdmi_main(void* thread_args) {
    // run indefinitely
    struct system_status *system = (system_status*) thread_args; 

    render(system);
    return NULL;
    
}