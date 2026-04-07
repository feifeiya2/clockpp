#include "st7789.h"

static ST7789_Interface_t st7789_interface;


// 发送8位命令
static void ST7789_WriteCommand(uint8_t cmd) {
    st7789_interface.set_dc_pin(0);
    st7789_interface.set_cs_pin(0);
    st7789_interface.write_spi_poll(&cmd, 1);
    st7789_interface.set_cs_pin(1);
}

// 发送8位数据
static void ST7789_WriteData(uint8_t data) {
    st7789_interface.set_dc_pin(1);
    st7789_interface.set_cs_pin(0);
    st7789_interface.write_spi_poll(&data, 1);
    st7789_interface.set_cs_pin(1);
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

//屏幕驱动初始化函数
void ST7789_Init(void) {
    st7789_interface.set_res_pin(0);
    st7789_interface.delay_ms(100);
    st7789_interface.set_res_pin(1);
    st7789_interface.delay_ms(120);
    st7789_interface.set_led_pin(1); // 点亮背光
    st7789_interface.delay_ms(100);

    ST7789_WriteCommand(0x11); // Sleep out
    st7789_interface.delay_ms(120);

    ST7789_WriteCommand(0x36); // MADCTL
    ST7789_WriteData(0xC0);    // 方向控制：可以尝试 0x00, 0x70, 0xC0 等

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

#include "main.h" // 确保能调用 CMSIS 指令 __REV16

void ST7789_Fill_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color) {
    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    
    // --- 关键：手动字节交换 ---
    // __REV16 是 Cortex-M4 的汇编指令：1个时钟周期完成高低字节翻转
    // 它能把 0x00F8 变成 0xF800
    for(uint32_t i = 0; i < size; i++) {
        color[i] = __REV16(color[i]);
    }
    
    // 设置窗口
    ST7789_SetAddressWindow(x1, y1, x2, y2);
    
    // 准备发送数据
    st7789_interface.set_dc_pin(1);
    st7789_interface.set_cs_pin(0);
    
    // 启动 DMA 发送
    // 此时数据已经是大端格式了，ST7789 收到后文字会非常清晰
    st7789_interface.write_spi_dma((uint8_t*)color, size * 2);
}
 
// void ST7789_Fill_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color) {
//     uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    
//     ST7789_SetAddressWindow(x1, y1, x2, y2);
    
//     st7789_interface.set_dc_pin(1);
//     st7789_interface.set_cs_pin(0);
    
//     // 因为SPI是8位模式，发送的数据长度需要 * 2 (16位颜色)
//     // 且 LVGL 默认颜色是小端，ST7789 需要大端。
//     // 如果颜色反了，可以在 LVGL 配置里开启 LV_COLOR_16_SWAP，或者在此手动转换。
//     st7789_interface.write_spi_dma((uint8_t*)color, size * 2);
// }

/**
 * @brief 全屏填充颜色
 * @param color: 16位RGB565颜色值 (例如: 0x0000 黑, 0xFFFF 白, 0xF800 红)
 */
void ST7789_Fill_Screen(uint16_t color) {
    // 1. 准备一行的颜色数据缓存 (240个像素 = 480字节)
    // 使用 static 避免占用过多的任务栈空间
    static uint16_t line_buffer[240];
    
    // 2. 处理字节序 (ST7789是高字节在前)
    uint16_t swapped_color = (color << 8) | (color >> 8);
    
    for (int i = 0; i < 240; i++) {
        line_buffer[i] = swapped_color;
    }

    // 3. 设置窗口为全屏 (0,0 到 239,319)
    ST7789_SetAddressWindow(0, 0, 239, 320 - 1);

    // 4. 开始发送数据
    st7789_interface.set_dc_pin(1);
    st7789_interface.set_cs_pin(0);

    for (int i = 0; i < 320; i++) {
        st7789_interface.write_spi_poll((uint8_t*)line_buffer, 240 * 2);
    }
    st7789_interface.set_cs_pin(1);
}

void ST7789_Register_IO(ST7789_Interface_t *driver_interface) {
    st7789_interface = *driver_interface;
}
