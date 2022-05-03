/**
 * @file button_client.c
 * @author Siddharth Chenrayan (chenrayan.s@northeastern.edu)
 * @brief Button press client. Now supports menu v2
 * @version 0.2
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#include "button_driver/zed_btns.h"
#include "../common_headers/cv_structs.h"
#include "../common_headers/system_management.h"
#include "../common_headers/button_client.h"
#include "../common_headers/menu_modes.h"

// global pollfd struct
struct pollfd zedbtns_pfd;

/////////////////////////////
/* Debug/print functions  */
/////////////////////////////

void print_sys_fzones(system_status* system) {
    int ii;
    camera_module* cam;
    puts("[ Btns ] - Camera data:");
    print_sys_menu_mode(system); 
    for (ii = 0; ii < system->numberOfCameras; ii++) {
        cam = system->cameras + ii;
        printf("\t");
        if (ii == system->guiState) {
            //active camera
            printf("*");
        }
        else {
            printf(" ");
        }

        printf("[%d] - { x: %d, y: %d, x-len: %d, y-len: %d, brightness: %d, contrast: %d }\n", 
            ii,
            cam->forbiddenZone.x_coord,
            cam->forbiddenZone.y_coord,
            cam->forbiddenZone.x_len,
            cam->forbiddenZone.y_len,
            cam->brightness,
            cam->contrast
        );
    }
}

void print_sys_menu_mode(system_status* system) {
    switch(system->mode) {
        case MODE_FZ_X:
            puts("[ Menu ] - Forbidden zone x-position mode");
            break; 
        case MODE_FZ_Y:
            puts("[ Menu ] - Forbidden zone y-position mode");
            break; 
        case MODE_FZ_XLEN:
            puts("[ Menu ] - Forbidden zone x-length mode");
            break;
        case MODE_FZ_YLEN:
            puts("[ Menu ] - Forbidden zone y-length mode");
            break;
        case MODE_CAM_BRIGHTNESS:
            puts("[ Menu ] - Camera brightness mode");
            break;
        case MODE_CAM_CONTRAST:
            puts("[ Menu ] - Camera contrast mode");
            break;
        default:
            puts("[ Menu ] - Unknown mode");
    }
}

/////////////////////////////
/* Button action functions */
/////////////////////////////

/* Menu mode actions */

void next_mode(system_status* system) {
    pthread_mutex_lock(&system->lock);
    system->mode++;
    if (system->mode == MODE_LAST) {
        system->mode = 0;
    }
    pthread_mutex_unlock(&system->lock);
#ifdef DEBUG
    print_sys_menu_mode(system);
#endif
}

void prev_mode(system_status* system) {
    pthread_mutex_lock(&system->lock);
    if (system->mode == 0) {
        system->mode = MODE_LAST - 1; // mode last is just a placeholder - the real "last" mode will always be MODE_LAST - 1
    }
    else {
        system->mode--;
    }
    pthread_mutex_unlock(&system->lock);
#ifdef DEBUG
    print_sys_menu_mode(system);
#endif
}

/* Forbidden zone functions */

void increment_fz_x(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        x_coord,
        FZ_POS_INC_DELTA,
        COORDINATE_MAX_X
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void decrement_fz_x(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        x_coord,
        FZ_POS_DEC_DELTA,
        COORDINATE_MAX_X
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void increment_fz_y(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        y_coord,
        FZ_POS_INC_DELTA,
        COORDINATE_MAX_Y
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void decrement_fz_y(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        y_coord,
        FZ_POS_DEC_DELTA,
        COORDINATE_MAX_Y
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void increment_fz_xlen(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        x_len,
        FZ_LEN_INC_DELTA,
        COORDINATE_MAX_X
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void decrement_fz_xlen(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        x_len,
        FZ_LEN_DEC_DELTA,
        COORDINATE_MAX_X
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void increment_fz_ylen(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        y_len,
        FZ_LEN_INC_DELTA,
        COORDINATE_MAX_Y
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void decrement_fz_ylen(system_status* system) {
    CHANGE_FZONE_PROPERTY(
        system,
        y_len,
        FZ_LEN_DEC_DELTA,
        COORDINATE_MAX_Y
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

/* Camera actions */ 

void cycle_active_camera(system_status* system) {
    pthread_mutex_lock(&system->lock);

    pause_stream(system->cameras[system->guiState].gstream_info);

    system->guiState++;
    if (system->guiState == system->numberOfCameras) {
        system->guiState = 0;
    }

    play_stream(system->cameras[system->guiState].gstream_info);

    pthread_mutex_unlock(&system->lock);
#ifdef DEBUG
    printf("[ Menu ] - New active camera: %d\n", system->guiState);
#endif
}

void increment_cam_brightness(system_status* system) {
    CHANGE_CAMERA_PROPERTY(
        system,
        brightness,
        CAM_BRIGHTNESS_INC_DELTA,
        CAM_BRIGHTNESS_MAX
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void decrement_cam_brightness(system_status* system) {
    CHANGE_CAMERA_PROPERTY(
        system,
        brightness,
        CAM_BRIGHTNESS_DEC_DELTA,
        CAM_BRIGHTNESS_MAX
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void increment_cam_contrast(system_status* system) {
    CHANGE_CAMERA_PROPERTY(
        system,
        contrast,
        CAM_BRIGHTNESS_INC_DELTA,
        CAM_CONTRAST_MAX
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

void decrement_cam_contrast(system_status* system) {
    CHANGE_CAMERA_PROPERTY(
        system,
        contrast,
        CAM_BRIGHTNESS_DEC_DELTA,
        CAM_CONTRAST_MAX
    );
#ifdef DEBUG
    print_sys_fzones(system);
#endif
}

/////////////////////////////
/* Button action structs */
/////////////////////////////

struct button_actions basic_menu_actions = {
    .on_center = cycle_active_camera,
    .on_down = increment_fz_y,
    .on_up = decrement_fz_y,
    .on_left = decrement_fz_x,
    .on_right = increment_fz_x
};

struct button_actions fz_x_actions = {
    .on_center = cycle_active_camera,
    .on_left = prev_mode,
    .on_right = next_mode, 
    .on_down = decrement_fz_x,
    .on_up = increment_fz_x,
};

struct button_actions fz_y_actions = {
    .on_center = cycle_active_camera,
    .on_left = prev_mode,
    .on_right = next_mode, 
    .on_down = decrement_fz_y,
    .on_up = increment_fz_y,
};

struct button_actions fz_xlen_actions = {
    .on_center = cycle_active_camera,
    .on_left = prev_mode,
    .on_right = next_mode, 
    .on_down = decrement_fz_xlen,
    .on_up = increment_fz_xlen,
};

struct button_actions fz_ylen_actions = {
    .on_center = cycle_active_camera,
    .on_left = prev_mode,
    .on_right = next_mode, 
    .on_down = decrement_fz_ylen,
    .on_up = increment_fz_ylen,
};

struct button_actions cam_brightness_actions = {
    .on_center = cycle_active_camera,
    .on_left = prev_mode,
    .on_right = next_mode, 
    .on_down = decrement_cam_brightness,
    .on_up = increment_cam_brightness,
};

struct button_actions cam_contrast_actions = {
    .on_center = cycle_active_camera,
    .on_left = prev_mode,
    .on_right = next_mode, 
    .on_down = decrement_cam_contrast,
    .on_up = increment_cam_contrast,
};

struct button_actions* menu_actions[MODE_LAST] = {
    [MODE_FZ_X] = &fz_x_actions,
    [MODE_FZ_Y] = &fz_y_actions,
    [MODE_FZ_XLEN] = &fz_xlen_actions,
    [MODE_FZ_YLEN] = &fz_ylen_actions,
    [MODE_CAM_BRIGHTNESS] = &cam_brightness_actions,
    [MODE_CAM_CONTRAST] = &cam_contrast_actions,
};

/////////////////////////////
/* Thread helpers */
/////////////////////////////

int initialize_buttons() {
    int fd = open(ZEDBTNS_FILE, O_RDONLY);
    if (fd == -1) {
        puts("[ Btns ] - Could not open ZEDBTNS_FILE");
        perror("[ Btns ]");
        return -1; 
    }

    zedbtns_pfd.fd = fd; 
    zedbtns_pfd.events = POLLIN;
    return 0;
}

void exec_action(struct button_actions** actions, int n_actions, button_value btn_val, struct system_status* system) {
    struct button_actions* actions_to_exec;
    enum menu_mode mode = system->mode;

    if (actions == NULL || system == NULL || mode >= n_actions) {
#ifdef DEBUG
        puts("[ Btns ] - Err when executing action: invalid args");
#endif
        return; 
    }

    actions_to_exec = actions[mode];

    if (IS_PRESSED(BTN_C, btn_val)) {
        actions_to_exec->on_center(system);
    }
    if (IS_PRESSED(BTN_D, btn_val)) {
        actions_to_exec->on_down(system);
    }
    if (IS_PRESSED(BTN_U, btn_val)) {
        actions_to_exec->on_up(system);
    }
    if (IS_PRESSED(BTN_L, btn_val)) {
        actions_to_exec->on_left(system);
    }
    if (IS_PRESSED(BTN_R, btn_val)) {
        actions_to_exec->on_right(system);
    }
}

void* run_button_client(void* thread_args) {
    int i; 
    button_value btn_val_buffer[BUTTON_BUFFER_MAX_SIZE];
    ssize_t bytes_read;
    system_status* system = (system_status*) thread_args; 

    puts("[ Btns ] - Hello from button listener thread!");

    while(system->running) {
#ifdef DEBUG
        puts("[ Btns ] - Going to sleep until button is pressed...");
#endif

        poll(&zedbtns_pfd, 1, -1);

#ifdef DEBUG
        puts("[ Btns ] - Woken up...");
#endif

        if (system->running && CAN_READ_PFD(zedbtns_pfd)) {
            bytes_read = read(zedbtns_pfd.fd, btn_val_buffer, BUTTON_BUFFER_MAX_SIZE);
#ifdef DEBUG
            printf("[ Btns ] - Read button values - bytes_read = %d\n", bytes_read);
#endif

            for (i = 0; i < BUTTON_BUFFER_MAX_SIZE && btn_val_buffer[i] != 0; i++) {
                exec_action(
                    menu_actions,
                    (int) MODE_LAST,
                    btn_val_buffer[i],
                    system
                );
            }
        }
    }

    close(zedbtns_pfd.fd);
    puts("[ Btns ] - Exiting button listener thread...");
    return NULL;
}
