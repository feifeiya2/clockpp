#ifndef __UART_DRIVER_H__
#define __UART_DRIVER_H__

#include "main.h"
#include "stdio.h"

extern UART_HandleTypeDef huart2;
// 缓冲区大小
#define RING_BUFFER_SIZE 1024

extern uint8_t data[100];

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint16_t head; // 指向下一个要写入的位置
    volatile uint16_t tail; // 指向下一个要读取的位置
} RingBuffer_t;
void uart_init(void);
void RingBuffer_Init(RingBuffer_t *rb);
uint8_t RingBuffer_IsEmpty(RingBuffer_t *rb);
uint8_t RingBuffer_IsFull(RingBuffer_t *rb);
uint8_t RingBuffer_Write(RingBuffer_t *rb, uint8_t data);
uint8_t RingBuffer_Read(RingBuffer_t *rb, uint8_t *data);
uint16_t RingBuffer_GetCount(RingBuffer_t *rb);

#endif /* __UART_DRIVER_H__ */
