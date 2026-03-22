#include "display_posting.h"

/*显示屏的IO函数的实现 start*/

static void Port_SPI_Write_Poll(const uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit(&ST7789_SPI_HANDLE, (uint8_t*)pData, Size, HAL_MAX_DELAY); 
}

static void Port_SPI_Write_DMA(const uint8_t *pData, uint16_t Size) {
    HAL_SPI_Transmit_DMA(&ST7789_SPI_HANDLE, (uint8_t*)pData, Size);
}

static void Port_Set_RES_Pin(uint8_t state) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Port_Set_DC_Pin(uint8_t state) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Port_Set_CS_Pin(uint8_t state) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Port_Set_LED_Pin(uint8_t state) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/*显示屏的IO函数的实现 end*/

/*注册显示屏的IO函数 start*/
static ST7789_Interface_t st7789_interface = {
        .write_spi_poll = HAL_SPI_Transmit,
        .write_spi_dma  = HAL_SPI_Transmit_DMA,
        .set_res_pin  = HAL_GPIO_WritePin,
        .set_dc_pin     = HAL_GPIO_WritePin,
        .set_cs_pin     = HAL_GPIO_WritePin,
        .set_led_pin    = HAL_GPIO_WritePin,
        .delay_ms = vTaskDelay,
}; 

void ST7789_Register_IO(void){


}
/*注册显示屏的IO函数 start*/
 
