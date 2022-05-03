/**
 * @file menu_modes.h
 * @author Siddharth Chenrayan (chenrayan.s@northeastern.edu)
 * @brief Defines actions for the various options configurable by button presses. 
 *        Includes actions for forbidden-zone position & size, and camera brightness and contrast
 * @version 0.1
 * @date 2022-04-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MENU_MODES_H
#define MENU_MODES_H

#include "system_management.h"

#define FZ_POS_INC_DELTA (10)
#define FZ_POS_DEC_DELTA (-10)

#define FZ_LEN_INC_DELTA (10)
#define FZ_LEN_DEC_DELTA (-10)

#define CAM_BRIGHTNESS_INC_DELTA (5)
#define CAM_BRIGHTNESS_DEC_DELTA (-5)

#define CAM_CONTRAST_INC_DELTA (5)
#define CAM_CONTRAST_DEC_DELTA (-5)

static inline uint16_t _apply_delta_enforce_range(uint16_t val, int8_t delta, uint16_t max) {
    if ((delta < 0) && (val <= -delta)) {
        return 0;
    }
    else if ((delta > 0) && (val + delta >= max)) {
        return max;
    } 
    else {
        return val + delta;
    }
}

/**
 * @brief Prints out the forbidden zone information of all cameras
 * 
 * @param system 
 */
void print_sys_fzones(system_status* system);

/**
 * @brief Prints out the camera brightness and contrast of all cameras
 * 
 * @param system 
 */
void print_sys_cameras(system_status* system);

/**
 * @brief Prints out the current menu mode of the system
 * 
 * @param system 
 */
void print_sys_menu_mode(system_status* system);

#define CHANGE_FZONE_PROPERTY(system, property, delta, max) \
do { \
    pthread_mutex_lock(&system->lock); \
    struct coordinate_data* _zone = &(system->cameras[system->guiState].forbiddenZone); \
    uint16_t _val = _apply_delta_enforce_range(_zone->property, delta, max); \
    _zone->property = _val; \
    pthread_mutex_unlock(&system->lock); \
} while(0);

#define CHANGE_CAMERA_PROPERTY(system, property, delta, max) \
do { \
    pthread_mutex_lock(&system->lock); \
    camera_module* _camera = &(system->cameras[system->guiState]); \
    uint16_t _val = _apply_delta_enforce_range(_camera->property, delta, max); \
    _camera->property = _val; \
    pthread_mutex_unlock(&system->lock); \
} while(0);

/**
 * @brief Modifies the menu mode with wraparound.
 * 
 * @param system - the overall system_status struct that houses all camera modules
 */
void prev_mode(system_status* system);
void next_mode(system_status* system);

/**
 * @brief Adds a delta to the forbidden zone coordinates of the current active camera.
 *        For increments (increment_fz_x and increment_fz_y): FZ_POS_INC_DELTA is used. 
 *        For decrements (decrement_fz_x and decrement_fz_y): FZ_POS_DEC_DELTA is used.
 *        Range is enforced such that the whole bounding box stays within the camera-viewport. 
 *        Currently, these values are hardcoded - COORDINATE_MAX_X and COORDINATE_MAX_Y.  
 * 
 * @param system - the overall system_status struct housing all the camera modules
 */
void increment_fz_x(system_status* system); 
void decrement_fz_x(system_status* system);
void increment_fz_y(system_status* system);
void decrement_fz_y(system_status* system);

/**
 * @brief Adds a delta to the forbidden zone length of the current active camera.
 *        For increments (increment_fz_xlen and increment_fz_ylen): FZ_INC_DELTA is used. 
 *        For decrements (decrement_fx_xlen and decrement_fz_ylen): FZ_DEC_DELTA is used.
 *        Range is enforced such that the whole bounding box stays within the camera-viewport. 
 *        Currently, these values are hardcoded - CAMERA_MAX_X and CAMERA_MAX_Y.  
 * 
 * @param system - the overall system_status struct housing all the camera modules
 */
void increment_fz_xlen(system_status* system);
void decrement_fz_xlen(system_status* system);
void increment_fz_ylen(system_status* system);
void decrement_fz_ylen(system_status* system);

/**
 * @brief Adds a delta to camera brightness value of the current active camera.
 *        For increments (increment_cam_brightness): FZ_INC_DELTA is used. 
 *        For decrements (decrement_cam_brightness): FZ_DEC_DELTA is used.
 *        Range is enforced such that the whole bounding box stays within the camera-viewport. 
 *        Currently, these values are hardcoded - CAMERA_MAX_X and CAMERA_MAX_Y.  
 * 
 * @param system - the overall system_status struct housing all the camera modules
 */
void increment_cam_brightness(system_status* system);
void decrement_cam_brightness(system_status* system);

/**
 * @brief Adds a delta to the camera contrast value of the current active camera.
 *        For increments (increment_cam_contrast): FZ_INC_DELTA is used. 
 *        For decrements (decrement_cam_contrast): FZ_DEC_DELTA is used.
 *        Range is enforced such that the value stays within 0 - 100.
 *        Currently, these values are hardcoded - CAMERA_MAX_X and CAMERA_MAX_Y.  
 * 
 * @param system - the overall system_status struct housing all the camera modules
 */
void increment_cam_contrast(system_status* system);
void decrement_cam_contrast(system_status* system);

/**
 * @brief Cycles the active camera. Increments guiState member, or wraps around to 0 after reaching last camera. 
 *        guiState member can be used as an index into cameras array.
 * 
 * @param system - the system_status containing the active camera state
 */
void cycle_active_camera(system_status* system);

#endif