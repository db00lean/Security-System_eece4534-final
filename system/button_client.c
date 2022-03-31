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

#include "button_client.h"
#include "button_driver/zed_btns.h"


char run_main = 1;

void stop_main(int _sig) {
    run_main = 0;
    puts("[ Btn Listener ] - Press any button on the ZedBoard to terminate process...");
}

struct button_actions debug_actions = {
    .on_center = print_center,
    .on_down = print_down,
    .on_up = print_up,
    .on_left = print_left,
    .on_right = print_right
};

int init_zedbtn_pollfd(struct pollfd* pfd) {
    int zedbtns_fd = open(ZEDBTNS_FILE, O_RDONLY);
    if (zedbtns_fd == -1) {
        return -1; 
    }

    pfd->fd = zedbtns_fd; 
    pfd->events = POLLIN | POLLRDNORM;
    return 0;
}

void flush_fd(int fd) {
    char buf[BUTTON_BUFFER_MAX_SIZE];
    ssize_t bytes_read;

    puts("[ Btn Listener ] - Flushing file contents...");
    bytes_read = read(fd, buf, BUTTON_BUFFER_MAX_SIZE);
    printf("[ Btn Listener ] - Flushed %d bytes from file.\n", bytes_read);
}

void exec_action(struct button_actions* actions, button_value btn_val) {
    if (IS_PRESSED(BTN_C, btn_val)) {
        actions->on_center(NULL);
    }

    if (IS_PRESSED(BTN_D, btn_val)) {
        actions->on_down(NULL);
    }

    if (IS_PRESSED(BTN_U, btn_val)) {
        actions->on_up(NULL);
    }

    if (IS_PRESSED(BTN_L, btn_val)) {
        actions->on_left(NULL);
    }

    if (IS_PRESSED(BTN_R, btn_val)) {
        actions->on_right(NULL);
    }
}

void* run_button_client(void* thread_args) {
    int err, i; 
    button_value btn_val_buffer[BUTTON_BUFFER_MAX_SIZE];
    ssize_t bytes_read;
    struct pollfd zedbtns_pfd;

    err = init_zedbtn_pollfd(&zedbtns_pfd);
    if (err) {
        puts("[ Btn Listener ] - Could not open zedbtn character device file.\n");
        return NULL;
    }
    flush_fd(zedbtns_pfd.fd);

    while(run_main) {
#ifdef DEBUG
        printf("[ Btn Listener ] - Going to sleep until button is pressed...\n");
#endif

        poll(&zedbtns_pfd, 1, -1);

#ifdef DEBUG
        printf("[ Btn Listener ] - Woken up...\n");
#endif

        if (run_main && CAN_READ_PFD(zedbtns_pfd)) {
            bytes_read = read(zedbtns_pfd.fd, btn_val_buffer, BUTTON_BUFFER_MAX_SIZE);
#ifdef DEBUG
            printf("[ Btn Listener ] - Read button values - bytes_read = %d\n", bytes_read);
#endif

            for (i = 0; i < BUTTON_BUFFER_MAX_SIZE && btn_val_buffer[i] != 0; i++) {
                exec_action(&debug_actions, btn_val_buffer[i]);  
            }
        }
    }

    flush_fd(zedbtns_pfd.fd);
    close(zedbtns_pfd.fd);
    puts("[ Btn Listener ] - Exiting button listener thread.");
    return NULL;
}

#ifdef BUTTON_CLIENT_MAIN
int main(int argc, char** argv) {
    pthread_t btn_listener_thread;

    signal(SIGINT, stop_main);
    pthread_create(&btn_listener_thread, NULL, run_button_client, NULL);

    puts("Hello from main button listener main!");
    // Can do other stuff in main...

    pthread_join(btn_listener_thread, NULL); 
    return 0; 
}
#endif
