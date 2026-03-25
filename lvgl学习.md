# 移植

### 第一步：获取源码并加入工程

1. **下载源码**：从 [LVGL GitHub](https://www.google.com/url?sa=E&q=https%3A%2F%2Fgithub.com%2Flvgl%2Flvgl%2Ftree%2Frelease%2Fv8.3) 下载 release/v8.3 分支。
2. **文件精简**：你的工程目录建议如下结构：
    - Middlewares/LVGL/src (保留全部)
    - Middlewares/LVGL/examples/porting (只需要其中的 lv_port_disp_template.c/h)
3. **IDE 设置**：
    - 将 src 下所有子目录的 .c 文件全部加入工程。
    - 添加头文件包含路径（Include Paths）：包含 lvgl/ 文件夹所在路径。

------



### 第二步：配置文件 lv_conf.h

1. 找到源码根目录的 lv_conf_template.h，复制一份到你的 Inc 文件夹下，改名为 **lv_conf.h**。
2. **修改内容**：
    - 第 15 行：#if 0 改为 **#if 1**。
    - 第 27 行：#define LV_COLOR_DEPTH 16 (确认是16)。
    - **关键点**（第 52 行左右）：#define LV_COLOR_16_SWAP 1 (必须设为 1，否则颜色还是反的)。
    - 第 88 行：#define LV_MEM_SIZE (32U * 1024U) (给 LVGL 分配 32KB 内存，F407 够大，可以给到 64KB)。

------



### 第三步：修改显示接口 lv_port_disp.c

找到刚才复制的 lv_port_disp_template.c，重命名为 lv_port_disp.c，并修改：

1. **包含头文件**：

    ```c
    #include "lv_port_disp.h"
    #include "st7789.h" // 包含你写的驱动头文件
    ```

2. **修改 lv_port_disp_init 函数**：
    选择一种缓冲方式（建议双缓冲，速度最快）：

    ```c
    void lv_port_disp_init(void) {
        static lv_disp_draw_buf_t draw_buf_dsc_1;
        // 定义两个缓冲区，每个 20 行像素高
        static lv_color_t buf_1[MY_DISP_HOR_RES * 20];
        static lv_color_t buf_2[MY_DISP_HOR_RES * 20];
        lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, buf_2, MY_DISP_HOR_RES * 20);
    
        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.hor_res = 240;
        disp_drv.ver_res = 240; // 根据你屏幕改
        disp_drv.flush_cb = disp_flush; // 绑定你的刷屏函数
        disp_drv.draw_buf = &draw_buf_dsc_1;
        lv_disp_drv_register(&disp_drv);
    }
    ```

3. **修改 disp_flush 函数**：

    ```c
    static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
        // 调用你之前写好的驱动函数
        // 记得确保这个函数内部使用的是 HAL_SPI_Transmit_DMA
        Display_Fill_Color(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);
        
        /* 注意：这里不要写 lv_disp_flush_ready！要在 DMA 中断里写 */
    }
    ```

------



### 第四步：打通 DMA 中断回调

在 main.c 或你的驱动文件里，找到 SPI 的 DMA 中断完成回调：

```c
// 这是一个全局变量，在 lv_port_disp.c 里定义一个 extern 指针指向 disp_drv 也可以
// 或者直接用 lv_disp_flush_ready(disp_drv)

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI2) {
        ST7789_CS_Set(); // 拉高片选
        
        // 关键：告诉 LVGL，这一块数据 DMA 发完了
        lv_disp_t * disp = _lv_refr_get_disp_refreshing();
        lv_disp_flush_ready(disp->driver);
    }
}
```

------



### 第五步：心跳与任务调度（FreeRTOS 版）

1. **提供心跳**：
    在 stm32f4xx_it.c 的 void SysTick_Handler(void) 中添加：

    ```c
    void SysTick_Handler(void) {
        HAL_IncTick();
        lv_tick_inc(1); // 每 1ms 给 LVGL 一个信号
    }
    ```

2. **配置 FreeRTOS 任务**：
    **重要：** 找到你的 StartDefaultTask 属性，把 Stack Size 改为 **1024 \* 4** (或者 4096 字节)。512 字节必爆栈。

    ```c
    void StartDefaultTask(void *argument) {
        lv_init();              // 1. LVGL 初始化
        lv_port_disp_init();    // 2. 显示接口初始化 (这内部会调 Display_Init)
    
        // 3. 简单测试：画一个标签
        lv_obj_t * label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "Hello LVGL!");
        lv_obj_center(label);
    
        for(;;) {
            lv_timer_handler(); // 4. LVGL 引擎运行
            osDelay(5);         // 5. 稍微延时，给其他任务留时间
        }
    }
    ```
