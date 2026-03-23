#include "display_posting.h"

/*显示屏的IO函数的实现 start*/

static void Port_SPI_Write_Poll(const uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit(&ST7789_SPI_HANDLE, (uint8_t*)pData, Size, HAL_MAX_DELAY); 
}

static void Port_SPI_Write_DMA(const uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit_DMA(&ST7789_SPI_HANDLE, (uint8_t*)pData, Size);
}           

static void Port_Set_RES_Pin(uint8_t state) {
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Port_Set_DC_Pin(uint8_t state) {
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Port_Set_CS_Pin(uint8_t state) {
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Port_Set_LED_Pin(uint8_t state) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/*显示屏的IO函数的实现 end*/

/*注册显示屏的接口工具 start*/
static ST7789_Interface_t st7789_interface = {
        .write_spi_poll = Port_SPI_Write_Poll,
        .write_spi_dma  = Port_SPI_Write_DMA,
        .set_res_pin  = Port_Set_RES_Pin,
        .set_dc_pin     = Port_Set_DC_Pin,
        .set_cs_pin     = Port_Set_CS_Pin,
        .set_led_pin    = Port_Set_LED_Pin,
        .delay_ms = vTaskDelay,
}; 

/*注册显示屏的接口工具 end*/

void Display_Posting(void) {
    // 将接口函数指针注册到ST7789驱动中
    ST7789_Register_IO(&st7789_interface);
}

