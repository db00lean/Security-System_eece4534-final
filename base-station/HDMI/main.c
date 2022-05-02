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

#include "inc/gstreamer-rx.h"
#include "inc/imagelib.h"
#include "../../common_headers/hdmi_main.h"
#include "../../common_headers/system_management.h"
#include "../common_headers/cv_structs.h"
#include "inc/imagelib.h"
#include "inc/DRM_user.h"

// IMPORTANT: 1 will run David's changes, 0 will run the old code
#define DAVID_CODE 1


#if DAVID_CODE
    #include "inc/draw.h"
#else
    #include "inc/draw_bounding_box.h"
    #include "inc/drawtext.h"
#endif

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

#if DAVID_CODE
    //top name
    draw_text(TITLE_X, TITLE_Y, "Security System", light_green, 4);

    //camera fram border
    static struct shapeObj * frm_bdr = NULL;
    if (!frm_bdr) frm_bdr = create_rect(IMAGE_WIDTH, IMAGE_HEIGHT, false, black, true, white);
    draw_shape(IMAGE_TOP_LEFT_X, IMAGE_TOP_LEFT_Y, frm_bdr, JUSTIFY_L, JUSTIFY_T);

    //shown box and people status block
    static struct shapeObj * opt_box = NULL;
    if (!opt_box) opt_box = create_rect(RIGHT_BOX_W, RIGHT_BOX_H, false, black, true, grey);
    static struct shapeObj * opt_cir = NULL;
    if (!opt_cir) opt_cir = create_cir(CAM_SEL_BOX_RAD, true, green, false, black);

    draw_shape(TOGGLE_OPT_BOX_TOP_LEFT_X, TOGGLE_OPT_BOX_TOP_LEFT_Y, opt_box, JUSTIFY_L, JUSTIFY_T);
    draw_text(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, TOGGLE_OPT_BOX_TOP_LEFT_Y + 50, "Show Boxes: ", light_green, 2);
    draw_shape(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2 + 100, TOGGLE_OPT_BOX_TOP_LEFT_Y + 50, opt_cir, JUSTIFY_C, JUSTIFY_C);
    draw_text(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, TOGGLE_OPT_BOX_TOP_LEFT_Y + 150, "Show Person #: ", light_green, 2);
    draw_shape(TOGGLE_OPT_BOX_TOP_LEFT_X + RIGHT_BOX_W/2 + 100, TOGGLE_OPT_BOX_TOP_LEFT_Y + 150, opt_cir, JUSTIFY_C, JUSTIFY_C);

    //people count block and label
    draw_shape(PEOPLE_BOX_TOP_LEFT_X, PEOPLE_BOX_TOP_LEFT_Y, opt_box, JUSTIFY_L, JUSTIFY_T);
    draw_text(PEOPLE_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, PEOPLE_BOX_TOP_LEFT_Y + 50, "Number of People", violet, 2);

    //zone status block and label
    draw_shape(ZONE_STATUS_TOP_LEFT_X, ZONE_STATUS_TOP_LEFT_Y, opt_box, JUSTIFY_L, JUSTIFY_T);
    draw_text(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 50, "Zone Status", violet, 2);

    //get the current camera information from the struct
    struct camera_module * active_camera = &system->cameras[system->guiState];

    //camera_boxes in bottom row
    char num_str[3];

    static struct shapeObj * cmr_box = NULL;
    if (!cmr_box) cmr_box = create_rect(CAM_SEL_BOX_DIM, CAM_SEL_BOX_DIM, true, grey, true, violet);

    for (int i = 0; i < system->numberOfCameras; i++) {
        draw_shape(CAM_SEL_BOX_X + (i * 200),
                   CAM_SEL_BOX_Y,
                   cmr_box,
                   JUSTIFY_L,
                   JUSTIFY_T);

        sprintf(num_str, "%d", i+1);
        draw_text(CAM_SEL_BOX_X + (i * 200) + (CAM_SEL_BOX_DIM / 2),
                  CAM_SEL_BOX_Y + (CAM_SEL_BOX_DIM / 2),
                  num_str, violet, 10);
    }

#else

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
    char num_str[10];

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

#endif
}

/**
 * @brief draws the current frame from active camera
 * 
 * @param system Struct holding system information
 */
void show_camera_frame(struct system_status * system) {
    // get the index of the active camera from the guistate
    // int active_camera_no = system->guiState;

    // pass the camera number to get the frame corresponding to the active camera number
    struct image * img = get_frame(system->cameras[0].gstream_info, IMGENC_ARGB, IMAGE_WIDTH, IMAGE_HEIGHT);

    // draw image to screen using draw pixel
    draw_map(IMAGE_TOP_LEFT_X, IMAGE_TOP_LEFT_Y, IMAGE_WIDTH, IMAGE_HEIGHT, (uint32_t*)img->buf);
    // for (int x = 0; x < IMAGE_WIDTH; x++) {
    //   for (int y = 0; y < IMAGE_HEIGHT; y++) {
    //         //uint32_t color = (img->buf[x + y + 0] << 16) | (img->buf[x + y + 1] << 8) | (img->buf[x + y + 2] << 0);
    //         uint32_t color = *((uint32_t*)img->buf + y * IMAGE_WIDTH + x);
    //         draw_pixel(IMAGE_TOP_LEFT_X + x,IMAGE_TOP_LEFT_Y + y,color);
    //     }
    // }

    // free the memory space of the frame
    free_image(img);
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

#if DAVID_CODE

    static struct shapeObj * bkgrnd = NULL;
    if (!bkgrnd) bkgrnd = create_rect(24, 24, true, black, false, black);

    draw_shape(PEOPLE_BOX_TOP_LEFT_X + RIGHT_BOX_W/2 - 12, PEOPLE_BOX_TOP_LEFT_Y + 88, bkgrnd, JUSTIFY_L, JUSTIFY_T);
    draw_text(PEOPLE_BOX_TOP_LEFT_X + RIGHT_BOX_W/2, PEOPLE_BOX_TOP_LEFT_Y + 100, (char *)&camera_num_display, 0x00ff00, 3);

    static struct shapeObj * bbox = NULL;
    if (!bbox) bbox = create_rect(0, 0, false, black, true, red);

    int b;
    for (b = 0; b < camera_metadata->num_bbox; b ++) {
        struct coordinate_data * box_data = &camera_metadata->box_data[b];
        bbox->dim1 = box_data->x_len;
        bbox->dim2 = box_data->y_len;
        draw_shape(box_data->x_coord,
                   box_data->y_coord,
                   bbox,
                   JUSTIFY_L,
                   JUSTIFY_T);
    }

#else

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

#endif
}

/**
 * @brief draws the camera information on the GUI
 * 
 * @param system Struct holding system information
 */
void show_camera_info(struct system_status * system) {

    // get the current camera information from the struct
    struct camera_module * active_camera = &system->cameras[system->guiState];

#if DAVID_CODE

    //zone status - dynamic
    if (active_camera->detection) {
        //first clear old text
        draw_text(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Vacant", black, 3);
        // draw "ZONE OCCUPIED"
        draw_text(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Occupied", red, 3);
    } else {
        //first clear old text
        draw_text(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Occupied", black, 3);
        // draw "ZONE VACANT"
        draw_text(ZONE_STATUS_TOP_LEFT_X + RIGHT_BOX_W/2, ZONE_STATUS_TOP_LEFT_Y + 100, "Vacant", green, 3);
    }

    static struct shapeObj * sel_box = NULL;
    if (!sel_box) sel_box = create_rect(CAM_SEL_BOX_DIM + 6, CAM_SEL_BOX_DIM + 6, false, black, true, red);

    static struct shapeObj * sts_cir = NULL;
    if (!sts_cir) sts_cir = create_cir(CAM_SEL_BOX_RAD, true, red, true, black);

    for (int i = 0; i < system->numberOfCameras; i++) {

        if (i == system->guiState) {
            draw_shape(CAM_SEL_BOX_X + (i * 200) - 3,
                       CAM_SEL_BOX_Y - 3,
                       sel_box,
                       JUSTIFY_L,
                       JUSTIFY_T);
        }
        else {
            draw_shape(CAM_SEL_BOX_X + (i * 200) - 3,
                       CAM_SEL_BOX_Y - 3,
                       sel_box,
                       JUSTIFY_L,
                       JUSTIFY_T);;        
        }
        //camera status circles
        if (system->cameras[i].status) {
            sts->fillColor = green;
        } else {
            sts->fillColor = red;
        }

        draw_shape(CAM_SEL_BOX_X + (i * 200) + (CAM_SEL_BOX_RAD / 2),
                   CAM_SEL_BOX_Y + (CAM_SEL_BOX_RAD / 2),
                   sts_cir,
                   JUSTIFY_C,
                   JUSTIFY_C);
    }

    // draw the forbidden zone as an overlay on the camera
    // TODO: coordinate translation between camera coordinates and HDMI coordinates
    struct coordinate_data * zone_data = &active_camera->forbiddenZone;

    static struct shapeObj * zone_box = NULL;
    if (!zone_box) zone_box = create_rect(zone_data->x_len, zone_data->y_len, false, black, true, grey);

    draw_shape(zone_data->x_coord, zone_data->y_coord, zone_box, JUSTIFY_L, JUSTIFY_T);

    // could draw other information, like number of people here

#else

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

#endif
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
    int fd;
    fd = drm_open();
    drm_init(fd);
    //map = drm_map(fd);

    print_info();

    //draw static elements
    show_background(system);
    changeActiveBuffer();
    show_background(system);
    changeActiveBuffer();

    //draw dynamic elements repeatedly
    while (1) {
        show_camera_frame(system);
        show_bounding_box(system);
        show_camera_info(system);
        show_camera_options(system);
        //g_usleep(166667);
        pageFlip();

    }
}

void* hdmi_main(void* thread_args) {
    // run indefinitely
    struct system_status *system = (system_status*) thread_args; 

    system->cameras[0].gstream_info = init_rx_camera("some string");

    render(system);

    cleanup_rx_camera(system->cameras[0].gstream_info);
    return NULL;
}
