#ifndef __ST7789_H
#define __ST7789_H

#include "main.h"

// --- 用户配置区域 ---
extern SPI_HandleTypeDef hspi2;   // 你的SPI句柄
#define ST7789_SPI_HANDLE    hspi2

#define ST7789_WIDTH  240
#define ST7789_HEIGHT 320

// 引脚定义 (根据你的实际接线修改)
#define ST7789_RES_GPIO_Port RESET_GPIO_Port
#define ST7789_RES_Pin       RESET_Pin
#define ST7789_DC_GPIO_Port  DC_GPIO_Port
#define ST7789_DC_Pin        DC_Pin
#define ST7789_CS_GPIO_Port  CS_GPIO_Port
#define ST7789_CS_Pin        CS_Pin
#define ST7789_LED_GPIO_Port  LED_GPIO_Port
#define ST7789_LED_Pin        LED_Pin

// 控制宏
#define ST7789_RES_Clr()  HAL_GPIO_WritePin(ST7789_RES_GPIO_Port, ST7789_RES_Pin, GPIO_PIN_RESET)
#define ST7789_RES_Set()  HAL_GPIO_WritePin(ST7789_RES_GPIO_Port, ST7789_RES_Pin, GPIO_PIN_SET)
#define ST7789_DC_Clr()   HAL_GPIO_WritePin(ST7789_DC_GPIO_Port, ST7789_DC_Pin, GPIO_PIN_RESET)
#define ST7789_DC_Set()   HAL_GPIO_WritePin(ST7789_DC_GPIO_Port, ST7789_DC_Pin, GPIO_PIN_SET)
#define ST7789_CS_Clr()   HAL_GPIO_WritePin(ST7789_CS_GPIO_Port, ST7789_CS_Pin, GPIO_PIN_RESET)
#define ST7789_CS_Set()   HAL_GPIO_WritePin(ST7789_CS_GPIO_Port, ST7789_CS_Pin, GPIO_PIN_SET)
#define ST7789_LED_Set()  HAL_GPIO_WritePin(ST7789_LED_GPIO_Port, ST7789_LED_Pin, GPIO_PIN_SET)

// 函数声明
void ST7789_Init(void);
void ST7789_Fill_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color);
void ST7789_Fill_Screen(uint16_t color);

#endif
