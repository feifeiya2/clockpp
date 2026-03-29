#ifndef __TOUCH_WRAPPER_H
#define __TOUCH_WRAPPER_H

#include "main.h"
#include "touch_posting.h"

typedef struct {
    uint8_t  is_pressed; // 0-抬起，1-按下
    uint16_t x;          // 通用X坐标
    uint16_t y;          // 通用Y坐标
} Touch_Data_t;

typedef struct{
    void (*Touch_Init)(void);
    int (*Touch_Read)(Touch_Data_t *state);
} Touch_Wrapper_t;

void Touch_Register_Wrapper(Touch_Wrapper_t *wrapper_interface);
void Touch_Init(void);
int Touch_Read(Touch_Data_t *state);

#endif /* __TOUCH_WRAPPER_H */

