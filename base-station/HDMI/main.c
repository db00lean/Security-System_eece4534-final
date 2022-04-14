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



    
    //shown box and people status block
    draw_boundingbox(1000, 500, 350, 100, 0xcccccc);
    draw_text_scale(1020, 512, "Show Boxes: ", 0x0ff00, 2);
    draw_circle_filled(1040, 512, 10, 0xff0000);
    draw_text_scale(1020, 550, "Show Person #: ", 0x00ff00, 2);
    draw_circle_filled(1040, 550, 10, 0xff0000);

    
    //camera status boxes
    draw_boundingbox(100, 750, 150, 150, 0xff0000);
    draw_text_scale(175, 825, "1", 0x00ffff, 10);
    draw_circle_filled(100, 750, 10, 0xff0000);


    draw_boundingbox(300, 750, 150, 150, 0xff0000);
    draw_text_scale(375, 825, "2", 0x00ffff, 10);
    draw_circle_filled(300, 750, 10, 0xff0000);


    draw_boundingbox(500, 750, 150, 150, 0xff0000);
    draw_text_scale(575, 825, "3", 0x00ffff, 10);
    draw_circle_filled(500, 750, 10, 0x00ff00);



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
    for (int x = 100; x < 900; x++) {
        for (int y = 100; y < 700; y++) {
            unsigned int color = (img->buf[x + y] << 16) | (img->buf[x + y + 1] << 8) | (img->buf[x + y + 2] << 0);
            draw_pixel(x,y,color);
        }
    }

    //draw bounding boxes
    draw_boundingbox(300, 300, 100, 200, 0xff0000);

    // //shape testing
    // draw_circle_filled(1300, 300, 200, 0xff00ff);
    // draw_circle_outline(300, 300, 200, 0xffff00);
    // draw_circle_outline(300, 300, 10, 0xffff00);
    // draw_rectangle_filled(1500, 700, 200, 200, 0xffff00);
    free_image(img);   
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
    draw_boundingbox(1000, 300, 200, 50, 0xcccccc);
    draw_text_scale(1000, 275, "Number of People", 0xff00ff, 2);
    int camera_num_display = camera_metadata->num_bbox + 48;
    draw_text_scale(1025, 312, (char *)&camera_num_display, 0x00ff00, 3);

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
    //zone status block
    draw_boundingbox(1000, 100, 200, 50, 0xcccccc);
    draw_text_scale(1000, 75, "Zone Status", 0xff00ff, 2);
    if (active_camera->detection) {
        // draw "ZONE OCCUPIED"
        draw_text_scale(1025, 112, "Occupied", 0x00ff00, 3);
    } else {
        // draw "ZONE VACANT"
        draw_text_scale(1025, 112, "Vacant", 0x00ff00, 3);

    }

    char num_str[2];
    int status_color;

    for (int i = 0; i < system->numberOfCameras) {

        if (i == system->guiState) {
            draw_boundingbox(CAM_SEL_BOX_X + (i * 200) - 3,
                             CAM_SEL_BOX_Y - 3,
                             CAM_SEL_BOX_DIM + 6,
                             CAM_SEL_BOX_DIM + 6,
                             bounding_box_colors[red]);
        }

        draw_rectangle_filled(CAM_SEL_BOX_X + (i * 200),
                              CAM_SEL_BOX_Y,
                              CAM_SEL_BOX_DIM,
                              CAM_SEL_BOX_DIM,
                              bounding_box_colors[grey]);

        sprintf(num_str, "%d", i);
        draw_text_scale(CAM_SEL_BOX_X + (i * 200) + (CAM_SEL_BOX_DIM / 2),
                        CAM_SEL_BOX_Y + (CAM_SEL_BOX_DIM / 2),
                        num_str,
                        bounding_box_colors[black], 10);

        if (system->cameras[i]->status) {
            status_color = bounding_box_colors[green];
        } else {
            status_color = bounding_box_colors[red];
        }

        draw_circle_filled(CAM_SEL_BOX_X + (i * 200) + (CAM_SEL_BOX_RAD / 2),
                           CAM_SEL_BOX_Y + (CAM_SEL_BOX_RAD / 2),,
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

    zone_data->x_coord = 100;
    zone_data->y_coord = 100;
    zone_data->x_len = 200;
    zone_data->y_len = 200;

    cv_dat_1->x_coord = 400;
    cv_dat_1->y_coord = 100;
    cv_dat_1->x_len = 100;
    cv_dat_1->y_len = 150;

    cv_dat_2->x_coord = 700;
    cv_dat_2->y_coord = 100;
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