#ifndef __ST7789_H
#define __ST7789_H

#include "main.h"

#define ST7789_WIDTH  240
#define ST7789_HEIGHT 320

typedef struct {
    void (*write_spi_poll)(const uint8_t *pData, uint16_t Size);        // 轮询方式写入SPI
    void (*write_spi_dma) (const uint8_t *pData, uint16_t Size);        // DMA方式写入SPI
    void (*set_res_pin)(uint8_t state);                               // 设置复位引脚
    void (*set_dc_pin)   (uint8_t state);                               // 设置数据/命令引脚
    void (*set_cs_pin)   (uint8_t state);                               // 设置片选引脚
    void (*set_led_pin)  (uint8_t state);                               // 设置背光引脚

    void (*delay_ms)     (uint32_t ms); 
} ST7789_Interface_t;

// 函数声明
void ST7789_Init(void);
void ST7789_Fill_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color);
void ST7789_Fill_Screen(uint16_t color);
void ST7789_Register_IO(ST7789_Interface_t *interface);

#endif
