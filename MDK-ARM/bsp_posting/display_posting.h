#ifndef __DISPLAY_POSTING_H
#define __DISPLAY_POSTING_H

#include "main.h"
#include "st7789.h"
#include "display_wrapper.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

extern SPI_HandleTypeDef hspi2;
extern ST7789_Interface_t interface;

#define ST7789_USE  // 定义使用ST7789显示屏

/*driver层 宏定义 start*/
#ifdef ST7789_USE
#define ST7789_SPI_HANDLE    hspi2
// 引脚定义
#define ST7789_RES_GPIO_Port RESET_GPIO_Port
#define ST7789_RES_Pin       RESET_Pin
#define ST7789_DC_GPIO_Port  DC_GPIO_Port
#define ST7789_DC_Pin        DC_Pin
#define ST7789_CS_GPIO_Port  CS_GPIO_Port
#define ST7789_CS_Pin        CS_Pin
#define ST7789_LED_GPIO_Port LED_GPIO_Port
#define ST7789_LED_Pin       LED_Pin
#endif
/*driver层 宏定义 end*/

void Display_Posting_Init(void);

#endif
