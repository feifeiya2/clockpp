#ifndef __FT6336_H
#define __FT6336_H

#include "main.h"
// I2C 设备地址 (FT6336 默认 7位地址是 0x38，左移一位得到 0x70)
#define FT6336_ADDR         (0x38 << 1)

// 寄存器定义
#define FT_REG_MODE         0x00    // 模式寄存器
#define FT_REG_NUM_TOUCHES  0x02    // 触摸点数量
#define FT_REG_X1_H         0x03    // 第一个坐标 X 高位
#define FT_REG_X1_L         0x04    // 第一个坐标 X 低位
#define FT_REG_Y1_H         0x05    // 第一个坐标 Y 高位
#define FT_REG_Y1_L         0x06    // 第一个坐标 Y 低位

// 触摸状态结构体
typedef struct {
    uint8_t  touched; // 0-未按下，1-按下
    uint16_t x;
    uint16_t y;
} FT6336_State_t;

// 引脚控制宏 (根据 CubeMX 命名的标签修改)
#define FT6336_RST_Clr()  HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_RESET)
#define FT6336_RST_Set()  HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, GPIO_PIN_SET)
#define FT6336_INT_Read() HAL_GPIO_ReadPin(T_INT_GPIO_Port, T_INT_Pin)

// 函数声明
uint8_t FT6336_Init(I2C_HandleTypeDef *hi2c);
uint8_t FT6336_Read_Touch(I2C_HandleTypeDef *hi2c, FT6336_State_t *state);

#endif /* __FT6336_H */
