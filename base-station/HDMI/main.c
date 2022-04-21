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
void show_background(struct system_status * system) {
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

    //people count block and label
    draw_boundingbox(PEOPLE_BOX_TOP_LEFT_X, PEOPLE_BOX_TOP_LEFT_Y, RIGHT_BOX_W, RIGHT_BOX_H, grey);
    draw_text_scale(PEOPLE_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, PEOPLE_BOX_TOP_LEFT_Y + 50, "Number of People", violet, 2);

    //zone status block and label
    draw_boundingbox(ZONE_STATUS_TOP_LEFT_X, ZONE_STATUS_TOP_LEFT_Y, RIGHT_BOX_W, RIGHT_BOX_H, grey);
    draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 50, "Zone Status", violet, 2);

    // get the current camera information from the struct
    struct camera_module * active_camera = &system->cameras[system->guiState];

    //camera boxes in bottom row
    char num_str[2];
    int status_color;

    for (int i = 0; i < system->numberOfCameras; i++) {
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
    }

    if (active_camera->status) {
        // draw "on"

    } else {
        // draw "off"
    }

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

    //people count number - dynamic
    int camera_num_display = camera_metadata->num_bbox + 48;
    draw_rectangle_filled(PEOPLE_BOX_TOP_LEFT_X + RIGHT_BOX_W/2 - 12, PEOPLE_BOX_TOP_LEFT_Y + 88, 24, 24, black);
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

    //zone status - dynamic
    if (active_camera->detection) {
        //first clear old text
        draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Vacant", black, 3);
        // draw "ZONE OCCUPIED"
        draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Occupied", red, 3);
    } else {
        //first clear old text
        draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Occupied", black, 3);
        // draw "ZONE VACANT"
        draw_text_scale(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Vacant", green, 3);
    }


    //active camera red box outline
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
        else {
                draw_boundingbox(CAM_SEL_BOX_X + (i * 200) - 3,
                             CAM_SEL_BOX_Y - 3,
                             CAM_SEL_BOX_DIM + 6,
                             CAM_SEL_BOX_DIM + 6,
                             black);        
        }
        //camera status circles
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

    //draw static elements
    show_background(system);

    //draw dynamic elements repeatedly
    while (1) {
        show_camera_frame(system);
        show_bounding_box(system);
        show_camera_info(system);
        show_camera_options(system);
        //page flip here
    }
}

int main() {
    // run indefinitely
    struct system_status * system = malloc(sizeof(struct system_status));
    struct camera_module * cameras = malloc(sizeof(struct camera_module) * 3);
    struct coordinate_data * zone_data = malloc(sizeof(struct coordinate_data));
    struct coordinate_data * cv_dat_1 = malloc(sizeof(struct coordinate_data));
    struct coordinate_data * cv_dat_2 = malloc(sizeof(struct coordinate_data));
    struct cv_data * computer_v = malloc(sizeof(struct cv_data));

    zone_data->x_coord = 120;
    zone_data->y_coord = 120;
    zone_data->x_len = 200;
    zone_data->y_len = 200;

    cv_dat_1->x_coord = 400;
    cv_dat_1->y_coord = 200;
    cv_dat_1->x_len = 100;
    cv_dat_1->y_len = 150;

    cv_dat_2->x_coord = 700;
    cv_dat_2->y_coord = 400;
    cv_dat_2->x_len = 100;
    cv_dat_2->y_len = 150;

    computer_v->num_bbox = 2;
    computer_v->box_data[0] = *cv_dat_1;
    computer_v->box_data[1] = *cv_dat_2;

    cameras[0].cameraNumber = 0;
    cameras[0].sysManPortNumber = 8080;
    cameras[0].streamPortNumber = 9090;

    cameras[0].status = 1;
    cameras[0].detection = 1;
    cameras[0].forbiddenZone = *zone_data;
    cameras[0].cvMetadata = *computer_v;

    system->numberOfCameras = 3;
    system->guiState = 0;
    system->cameras = cameras;

    render(system);
    
}