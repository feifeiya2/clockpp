#ifndef __DISPLAY_WRAPPER_H
#define __DISPLAY_WRAPPER_H

#include "stdint.h"

typedef struct{
    void (*Display_Init)(void);
    void (*Display_Fill_Color)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color);
    void (*Display_Fill_Screen)(uint16_t color);
} Display_Wrapper_t;

/*提供给上层的屏幕操作函数 start*/
void Display_Init(void);
void Display_Fill_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color);
void Display_Fill_Screen(uint16_t color);
/*提供给上层的屏幕操作函数 end*/

//wrapper接口注册函数
void Display_Register_Wrapper(Display_Wrapper_t *wrapper_interface);
#endif
