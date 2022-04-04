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
#include "button_driver/zed_btns.h"

#define CAN_READ_PFD(pfd) (pfd.revents & POLLIN)

typedef void (*button_action)(void* args);
typedef uint8_t button_value;

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
 * @brief Flushes the current contents of the given fd, with logging. 
 * 
 * @param fd - file descriptor of the file to flush
 */
void flush_fd(int fd);

/**
 * @brief Executes actions from the given set of button_actions, corresponding to what button(s) have been pressed. 
 * 
 * @param actions - pointer to a single set of actions to execute,
 * @param btn_val - the current button values to interpret. 
 *                  Refer to zed_btns.h and the IS_PRESSED macro to interpret this value 
 */
void exec_action(struct button_actions* actions, button_value btn_val);

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

/* Debug/print functions for a basic set of actions */
static void print_center(void* _args) {
    puts("Center button pressed!");
}

static void print_up(void* _args) {
    puts("Up button pressed!");
}

static void print_down(void* _args) {
    puts("Down button pressed!");
}

static void print_left(void* _args) {
    puts("Left button pressed!");
}

static void print_right(void* _args) {
    puts("Right button pressed!");
}

#endif