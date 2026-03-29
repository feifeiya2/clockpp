#include "ft6336.h"

static FT6336_Interface_t ft6336_interface;

/**
 * @brief 注册触摸芯片操作接口
 * @param interface 指向 FT6336_Interface_t 结构体的指针
 */
void FT6336_register_interface(FT6336_Interface_t *interface) {
    ft6336_interface = *interface;
}

/**
 * @brief 复位并初始化 FT6336
 */
uint8_t FT6336_Init(void) {
    ft6336_interface.set_res_pin(0);
    ft6336_interface.delay_ms(10);
    ft6336_interface.set_res_pin(1);
    ft6336_interface.delay_ms(100);

    uint8_t id = 0;
    // 尝试读取 ID 或 模式寄存器，检查通信是否正常
    if (ft6336_interface.i2c_mem_read(FT6336_ADDR, FT_REG_MODE, &id, 1, 100) != HAL_OK) {
        return 0; // 初始化失败
    }
    return 1; // 初始化成功
}

/**
 * @brief 读取当前触摸坐标
 * @return 1: 有触摸, 0: 无触摸
 */
uint8_t FT6336_Read_Touch(FT6336_State_t *state) {
    uint8_t data[6]; // 用于存放读取的 0x02~0x07 寄存器数据
    
    // 连续读取从 0x02 开始的 6 个字节
    if (ft6336_interface.i2c_mem_read(FT6336_ADDR, FT_REG_NUM_TOUCHES, data, 6, 100) != HAL_OK) {
        state->touched = 0;
        return 0;
    }

    // data[0] 是触摸点数量
    uint8_t touch_points = data[0] & 0x0F;

    if (touch_points == 0) {
        state->touched = 0;
        return 0;
    }

    // 解析坐标数据 (注意位操作)
    // FT6336 的 X 和 Y 坐标是由高 4 位和低 8 位拼接而成的 12 位数据
    state->x = ((uint16_t)(data[1] & 0x0F) << 8) | data[2];
    state->y = ((uint16_t)(data[3] & 0x0F) << 8) | data[4];
    state->touched = 1;

    // 旋转 180°

    state->x = 240 - state->x;
    state->y = 320 - state->y;
    
    // 如果 X/Y 轴反了，交换一下:
    // uint16_t temp = state->x; state->x = state->y; state->y = temp;

    return 1;
}
