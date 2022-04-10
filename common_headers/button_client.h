/**
 * @file button_client.h
 * @author Siddharth Chenrayan (chenrayan.s@northeastern.edu)
 * @brief Header file for button client
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BUTTON_CLIENT_H
#define BUTTON_CLIENT_H

#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>

#include "../base-station/button_driver/zed_btns.h"
#include "system_management.h"

#define FZ_INC_DELTA (1)
#define FZ_DEC_DELTA (-1)

#define CAN_READ_PFD(pfd) (pfd.revents & POLLIN)

#define ENFORCE_RANGE(val, min, max) \
do { \
    if (val < min) \
        val = min; \
    else if (val > max) \
        val = max; \
} while(0);

typedef void (*button_action)(struct system_status* args);
typedef uint8_t button_value;

/**
 * struct button_actions - holds callbacks to button actions
 * 
 * Members: 
 * @on_center - Callback fired when center button is pressed
 * @on_left - Callback fired when left button is pressed
 * @on_right - Callback fired when right button is pressed
 * @on_up - Callback fired when up button is pressed
 * @on_down - Callback fired when down button pressed
 */
struct button_actions {
    button_action on_center; 
    button_action on_left; 
    button_action on_right; 
    button_action on_up; 
    button_action on_down;
};

/**
 * @brief Initializes the given pollfd struct to listen to read events on the zedbtns character device file
 *        defined by the ZEDBTNS_FILE macro
 * 
 * @param pfd - pointer to the pollfd struct to initialize
 * 
 * @return 0 on success, -1 if zedbtns file could not be opened.
 */
int init_zedbtn_pollfd(struct pollfd* pfd);

/**
 * @brief Executes actions from the given set of button_actions, corresponding to what button(s) have been pressed. 
 * 
 * @param actions - pointer to a single set of actions to execute,
 * @param n_actions - number of actions in the given list of actions, used for bounds checking
 * @param btn_val - the current button values to interpret. 
 *                  Refer to zed_btns.h and the IS_PRESSED macro to interpret this value 
 * @param system - pointer to the system_status struct, passed as an argument to the button action.
 */
void exec_action(struct button_actions* actions, int n_actions, button_value btn_val, struct system_status* system);

/**
 * @brief Button press listener. Meant to be used as a thread function (argument to pthread_create)
 * 
 * @param thread_args - args to the thread function
 * 
 * What should be passed in?
 * - List of button/menu actions that can be executed. 
 * - Pointer to global state struct(s) that can be modified by various actions. Prevent data races -> need mutexes?
 * - Anything else useful? 
 */
void* run_button_client(void* thread_args);

/**
 * @brief Stops execution of the button listener thread in response to SIGINT (or any other signal, really). 
 *        Make sure to pass this to the signal syscall (see man 2 signal)
 *        Otherwise, button listener thread will not clean up properly!
 * 
 * @param _sig - Signal (as an integer). Unused, hence the prefix with "_" 
 */
void stop_button_listener(int _sig);

void increment_fz_x(system_status* system); 

void decrement_fz_x(system_status* system);

void increment_fz_y(system_status* system);

void decrement_fz_y(system_status* system);

void increment_active_camera(system_status* system);

/* Debug/print functions for a basic set of actions */
static void print_center(struct system_status* _args) {
    puts("Center button pressed!");
}

static void print_up(struct system_status* _args) {
    puts("Up button pressed!");
}

static void print_down(struct system_status* _args) {
    puts("Down button pressed!");
}

static void print_left(struct system_status* _args) {
    puts("Left button pressed!");
}

static void print_right(struct system_status* _args) {
    puts("Right button pressed!");
}

#endif