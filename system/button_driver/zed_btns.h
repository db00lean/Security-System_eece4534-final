#ifndef ZED_BTNS_H
#define ZED_BTNS_H

#define BTN_C (1 << 0)
#define BTN_D (1 << 1)
#define BTN_L (1 << 2)
#define BTN_R (1 << 3)
#define BTN_U (1 << 4)

#define IS_PRESSED(btn, btn_val) (btn & btn_val)

#endif
