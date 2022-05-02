/**
 * @file hdmi_main.h
 * @author HDMI team
 * @brief hdmi functions accessible to base-station server. 
 * @version 0.1
 * @date 2022-04-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef HDMI_MAIN
#define HDMI_MAIN

/**
 * @brief Initializes HDMI connection and DRM
 * 
 * @return int Non-zero if error setting up hdmi.
 */
int initialize_hdmi();

/**
 * @brief HDMI main entry point function
 * 
 * @param thread_args system_status struct of main security_system
 * @return void* NULL when thread exits
 */
void* hdmi_main(void* thread_args);
#endif
