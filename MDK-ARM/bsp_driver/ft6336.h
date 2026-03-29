#ifndef __FT6336_H
#define __FT6336_H

#include "main.h"

typedef enum 
{
  FT6336_OK       = 0x00U,
  FT6336_ERROR    = 0x01U,
  FT6336_BUSY     = 0x02U,
  FT6336_TIMEOUT  = 0x03U
} FT6336_Status;

typedef struct {
    FT6336_Status (*i2c_mem_read)(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);        // 轮询方式读I2C
    void (*set_res_pin)(uint8_t state);                               // 设置复位引脚
    void (*delay_ms)     (uint32_t ms); 
} FT6336_Interface_t;

// I2C 设备地址 (FT6336 默认 7位地址是 0x38，左移一位得到 0x70)
#define FT6336_ADDR         (0x38 << 1)

// 寄存器定义
#define FT_REG_MODE         0x00    // 模式寄存器
#define FT_REG_NUM_TOUCHES  0x02    // 触摸点数量
#define FT_REG_X1_H         0x03    // 第一个坐标 X 高位
#define FT_REG_X1_L         0x04    // 第一个坐标 X 低位
#define FT_REG_Y1_H         0x05    // 第一个坐标 Y 高位
#define FT_REG_Y1_L         0x06    // 第一个坐标 Y 低位

// 触摸数据结构体
typedef struct {
    uint8_t  touched; // 0-未按下，1-按下
    uint16_t x;
    uint16_t y;
} FT6336_Data_t;

// 函数声明
void FT6336_Init(void);
uint8_t FT6336_Read_Touch(FT6336_Data_t *state);
void FT6336_register_interface(FT6336_Interface_t *interface);

#endif /* __FT6336_H */
