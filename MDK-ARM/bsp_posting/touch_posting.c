#include "touch_posting.h"

/*  <Driver层>  的IO函数的实现 start*/

static FT6336_Status Port_I2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout){// 轮询方式读I2C
    HAL_StatusTypeDef res = HAL_I2C_Mem_Read(&FT6336_I2C_HANDLE, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, Timeout);
    return (FT6336_Status)res;
}       

static void Port_I2C_Res_pin(uint8_t state){
    HAL_GPIO_WritePin(FT6336_RST_GPIO_Port, FT6336_RST_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void Port_Delay_ms(uint32_t ms){
    vTaskDelay(ms);
}

/*  <Driver层>  的IO函数的实现 end*/

/*  触摸屏数据转义函数 */
static int Touch_Data_Translate(Touch_Data_t *touch_data) {
    FT6336_Data_t ft6336_data;
    FT6336_Read_Touch(&ft6336_data);
    touch_data->is_pressed = ft6336_data.touched;
    touch_data->x = ft6336_data.x;
    touch_data->y = ft6336_data.y;

    //旋转180°
    touch_data->x = 240 - touch_data->x;
    touch_data->y = 320 - touch_data->y;

    if(touch_data->is_pressed == 1){
        return 1;
    }else{
        return 0;
    }
}

/*注册触摸屏Driver层的接口工具*/
static FT6336_Interface_t ft6336_interface = {
    .i2c_mem_read = Port_I2C_Mem_Read,
    .set_res_pin = Port_I2C_Res_pin,
    .delay_ms = Port_Delay_ms,
};

/*注册触摸屏Wrapper层的接口工具*/
static Touch_Wrapper_t touch_wrapper_interface = {
    .Touch_Init = FT6336_Init,
    .Touch_Read = Touch_Data_Translate,
};


/*Posting层初始化函数*/
void Touch_Posting_Init(void) {
    // 将接口函数指针注册到FT6336驱动中
    FT6336_register_interface(&ft6336_interface);
    // 将FT6336的显示函数注册到Wrapper层
    Touch_Register_Wrapper(&touch_wrapper_interface);

}
