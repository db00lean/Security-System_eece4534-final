/**
 * @file button_client.c
 * @author Siddharth Chenrayan (chenrayan.s@northeastern.edu)
 * @brief Button press client
 * @version 0.1
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

// global pollfd struct
struct pollfd zedbtns_pfd;

/* Debug/print functions for a basic set of button actions */

void print_sys_fzones(system_status* system) {
    int ii;
    camera_module* cam;  
    for (ii = 0; ii < system->numberOfCameras; ii++) {
        cam = system->cameras + ii;
        printf("\t[%d] - { X: %d, Y: %d }\n", ii, cam->forbiddenZone.x_coord, cam->forbiddenZone.y_coord);
    }
}

void print_center(struct system_status* _args) {
    puts("Center button pressed!");
}

void print_up(struct system_status* _args) {
    puts("Up button pressed!");
}

void print_down(struct system_status* _args) {
    puts("Down button pressed!");
}

void print_left(struct system_status* _args) {
    puts("Left button pressed!");
}

void print_right(struct system_status* _args) {
    puts("Right button pressed!");
}

/* Button action helpers */

void change_fz_x(system_status* system, int8_t delta) {
     pthread_mutex_lock(&system->lock);
    
    struct coordinate_data* zone = &(system->cameras[system->guiState].forbiddenZone);
    APPLY_DELTA_ENFORCE_RANGE(zone->x_coord, delta, (COORDINATE_MAX_X - zone->x_len));

    pthread_mutex_unlock(&system->lock);
#ifdef DEBUG
    printf("[ Menu ] - Change forbidden zone x-coord; Delta: %d, New value: %u\n", delta, zone->x_coord);
    print_sys_fzones(system);
#endif
}

void change_fz_y(system_status* system, int8_t delta) {
    pthread_mutex_lock(&system->lock);
    
    struct coordinate_data* zone = &(system->cameras[system->guiState].forbiddenZone);
    APPLY_DELTA_ENFORCE_RANGE(zone->y_coord, delta, (COORDINATE_MAX_Y - zone->y_len));

    pthread_mutex_unlock(&system->lock);
#ifdef DEBUG
    printf("[ Menu ] - Change forbidden zone y-coord; Delta: %d, New value: %u\n", delta, zone->y_coord);
    print_sys_fzones(system);
#endif
}

/* Button action functions */

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
    printf("[ Menu ] - Increment active camera; New active camera: %d\n", system->guiState);
#endif
}

void increment_fz_x(system_status* system) {
    change_fz_x(system, FZ_INC_DELTA);
}

void decrement_fz_x(system_status* system) {
    change_fz_x(system, FZ_DEC_DELTA);
}

void increment_fz_y(system_status* system) {
    change_fz_y(system, FZ_INC_DELTA); 
}

void decrement_fz_y(system_status* system) {
    change_fz_y(system, FZ_DEC_DELTA); 
}

/* Button action Structs */

struct button_actions debug_actions = {
    .on_center = print_center,
    .on_down = print_down,
    .on_up = print_up,
    .on_left = print_left,
    .on_right = print_right
};

struct button_actions basic_menu_actions = {
    .on_center = cycle_active_camera,
    .on_down = increment_fz_y,
    .on_up = decrement_fz_y,
    .on_left = decrement_fz_x,
    .on_right = increment_fz_x
};

/* Thread setup and helpers */

int initialize_buttons() {
    int fd = open(ZEDBTNS_FILE, O_RDONLY);
    if (fd == -1) {
        puts("[ Btns ] - Could not open ZEDBTNS_FILE");
        return -1; 
    }

    zedbtns_pfd.fd = fd; 
    zedbtns_pfd.events = POLLIN;
    return 0;
}

void exec_action(struct button_actions* actions, int n_actions, button_value btn_val, struct system_status* system) {
    struct button_actions actions_to_exec;
    uint8_t mode = system->menuMode;

    if (actions == NULL || system == NULL || mode >= n_actions) {
#ifdef DEBUG
        puts("[ Btns ] - Err when executing action: invalid args");
#endif
        return; 
    }

    actions_to_exec = actions[mode];

    if (IS_PRESSED(BTN_C, btn_val)) {
        actions_to_exec.on_center(system);
    }
    if (IS_PRESSED(BTN_D, btn_val)) {
        actions_to_exec.on_down(system);
    }
    if (IS_PRESSED(BTN_U, btn_val)) {
        actions_to_exec.on_up(system);
    }
    if (IS_PRESSED(BTN_L, btn_val)) {
        actions_to_exec.on_left(system);
    }
    if (IS_PRESSED(BTN_R, btn_val)) {
        actions_to_exec.on_right(system);
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
                exec_action(&basic_menu_actions, 1, btn_val_buffer[i], system);
            }
        }
    }

    close(zedbtns_pfd.fd);
    puts("[ Btns ] - Exiting button listener thread...");
    return NULL;
}
