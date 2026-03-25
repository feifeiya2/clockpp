#include "display_wrapper.h"

static Display_Wrapper_t display_wrapper;

void Display_Register_Wrapper(Display_Wrapper_t *wrapper_interface) {
    display_wrapper = *wrapper_interface;
}

void Display_Init(void) {
    Display_Posting_Init(); // 确保Posting层初始化，注册接口
    if(display_wrapper.Display_Init == NULL){
        // TODO: error handling
        return;
    }

    display_wrapper.Display_Init();
}

void Display_Fill_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color) {
    if(display_wrapper.Display_Fill_Color == NULL){
        // TODO: error handling
        return;
    }
    display_wrapper.Display_Fill_Color(x1, y1, x2, y2, color);
}

void Display_Fill_Screen(uint16_t color) {
    if(display_wrapper.Display_Fill_Screen == NULL){
        // TODO: error handling
        return;
    }
    display_wrapper.Display_Fill_Screen(color);
}
