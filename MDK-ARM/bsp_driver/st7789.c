#include "st7789.h"

// 发送8位命令
static void ST7789_WriteCommand(uint8_t cmd) {
    ST7789_DC_Clr();
    ST7789_CS_Clr();
    HAL_SPI_Transmit(&ST7789_SPI_HANDLE, &cmd, 1, HAL_MAX_DELAY);
    ST7789_CS_Set();
}

// 发送8位数据
static void ST7789_WriteData(uint8_t data) {
    ST7789_DC_Set();
    ST7789_CS_Clr();
    HAL_SPI_Transmit(&ST7789_SPI_HANDLE, &data, 1, HAL_MAX_DELAY);
    ST7789_CS_Set();
}

// 设置显示窗口
void ST7789_SetAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    ST7789_WriteCommand(0x2A); // Column address set
    ST7789_WriteData(x1 >> 8);
    ST7789_WriteData(x1 & 0xFF);
    ST7789_WriteData(x2 >> 8);
    ST7789_WriteData(x2 & 0xFF);

    ST7789_WriteCommand(0x2B); // Row address set
    ST7789_WriteData(y1 >> 8);
    ST7789_WriteData(y1 & 0xFF);
    ST7789_WriteData(y2 >> 8);
    ST7789_WriteData(y2 & 0xFF);

    ST7789_WriteCommand(0x2C); // Memory write
}

void ST7789_Init(void) {
    ST7789_RES_Clr();
    HAL_Delay(100);
    ST7789_RES_Set();
    HAL_Delay(120);

    ST7789_LED_Set(); // 点亮背光
    HAL_Delay(100);

    ST7789_WriteCommand(0x11); // Sleep out
    HAL_Delay(120);

    ST7789_WriteCommand(0x36); // MADCTL
    ST7789_WriteData(0x00);    // 方向控制：可以尝试 0x00, 0x70, 0xC0 等

    ST7789_WriteCommand(0x3A); // COLMOD
    ST7789_WriteData(0x05);    // 16bit RGB565

    // 以下是优化后的常用序列，省略部分具体参数调优...
    ST7789_WriteCommand(0x20); // Display Inversion On (ST7789 很多是 IPS，需要开启反转)
    ST7789_WriteCommand(0x29); // Display On


    
}

/**
 * 核心函数：使用DMA填充颜色
 * 用于对接 LVGL 的 flush_cb
 */
void ST7789_Fill_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color) {
    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    
    ST7789_SetAddressWindow(x1, y1, x2, y2);
    
    ST7789_DC_Set();
    ST7789_CS_Clr();
    
    // 因为SPI是8位模式，发送的数据长度需要 * 2 (16位颜色)
    // 且 LVGL 默认颜色是小端，ST7789 需要大端。
    // 如果颜色反了，可以在 LVGL 配置里开启 LV_COLOR_16_SWAP，或者在此手动转换。
    HAL_SPI_Transmit_DMA(&ST7789_SPI_HANDLE, (uint8_t*)color, size * 2);
}


/**
 * @brief 全屏填充颜色
 * @param color: 16位RGB565颜色值 (例如: 0x0000 黑, 0xFFFF 白, 0xF800 红)
 */
void ST7789_Fill_Screen(uint16_t color) {
    // 1. 准备一行的颜色数据缓存 (240个像素 = 480字节)
    // 使用 static 避免占用过多的任务栈空间
    static uint16_t line_buffer[240];
    
    // 2. 处理字节序 (ST7789是高字节在前)
    // 如果是 0x0000 (黑色)，交换后还是 0x0000，不需要处理
    // 如果是其他颜色，需要交换高低字节
    uint16_t swapped_color = (color << 8) | (color >> 8);
    
    for (int i = 0; i < 240; i++) {
        line_buffer[i] = swapped_color;
    }

    // 3. 设置窗口为全屏 (0,0 到 239,319)
    ST7789_SetAddressWindow(0, 0, 239, 320 - 1);

    // 4. 开始发送数据
    ST7789_DC_Set();
    ST7789_CS_Clr();

    for (int i = 0; i < 320; i++) {
        // 循环发送 240 次，每次发送一行 (480 字节)
        // 这里建议先用阻塞模式 HAL_SPI_Transmit，确保初始化清屏成功
        HAL_SPI_Transmit(&ST7789_SPI_HANDLE, (uint8_t*)line_buffer, 240 * 2, HAL_MAX_DELAY);
    }

    ST7789_CS_Set();
}
