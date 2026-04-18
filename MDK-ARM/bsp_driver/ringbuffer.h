#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include "stdint.h"

/**
 * @brief 环形缓冲区结构体定义
 */
typedef struct {
    uint8_t *buffer;      // 缓冲区空间指针
    uint16_t size;        // 缓冲区总大小
    volatile uint16_t head; // 写入位置索引
    volatile uint16_t tail; // 读取位置索引
} RingBuffer;

/* ----- 基础管理函数 ----- */

// 初始化缓冲区
void RingBuffer_Init(RingBuffer *rb, uint8_t *pool, uint16_t size);

// 清空缓冲区
void RingBuffer_Clear(RingBuffer *rb);

// 获取已存储的数据长度
uint16_t RingBuffer_GetCount(RingBuffer *rb);

// 获取剩余可用空间长度
uint16_t RingBuffer_GetFree(RingBuffer *rb);

/* ----- 数据操作函数 (支持回绕处理) ----- */

/**
 * @brief 批量写入数据到缓冲区
 * @param rb 缓冲区结构体指针
 * @param data 要写入的数据源地址
 * @param len 要写入的长度
 * @return 实际写入的长度
 */
uint16_t RingBuffer_Write(RingBuffer *rb, const uint8_t *data, uint16_t len);

/**
 * @brief 从缓冲区批量读取数据
 * @param rb 缓冲区结构体指针
 * @param dest 存放数据的目标地址
 * @param len 想要读取的最大长度
 * @return 实际读取到的长度
 */
uint16_t RingBuffer_Read(RingBuffer *rb, uint8_t *dest, uint16_t len);

/**
 * @brief  【雷达功能】扫描缓冲区，寻找指定的结束字符
 * @param  rb: 环形缓冲区指针
 * @param  target_char: 要寻找的目标字符
 * @retval >0: 找到了！返回包含目标字符在内的完整长度 (可以直接用于 Read)
 *         0: 没找到，或者缓冲区为空
 */
uint16_t RingBuffer_Find_Char(RingBuffer *rb, uint8_t target_char);

#endif /* __RINGBUFFER_H__ */
