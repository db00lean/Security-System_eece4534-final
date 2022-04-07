/**
 * @file zed_btns.h
 * @author Siddharth Chenrayan (chenrayan.s@northeastern.edu)
 * @brief General purpose header file for ZedBoard button interface. Used by kernel module and button-listener
 * @version 0.1
 * @date 2022-03-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ZED_BTNS_H
#define ZED_BTNS_H

#define BTN_C (1 << 0)
#define BTN_D (1 << 1)
#define BTN_L (1 << 2)
#define BTN_R (1 << 3)
#define BTN_U (1 << 4)

#define IS_PRESSED(btn, btn_val) (btn & btn_val)

#define ZEDBTNS_FILE "/dev/zedbtns0"
#define BUTTON_BUFFER_MAX_SIZE 32

#endif
