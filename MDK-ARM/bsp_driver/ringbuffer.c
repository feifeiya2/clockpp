#include "ringbuffer.h"
#include "stdio.h"
#include <stdbool.h>
#include <string.h>

void RingBuffer_Init(RingBuffer *rb, uint8_t *pool, uint16_t size) {
    rb->buffer = pool;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
}

void RingBuffer_Clear(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
}

uint16_t RingBuffer_GetCount(RingBuffer *rb) {
    uint16_t h = rb->head;
    uint16_t t = rb->tail;
    if (h >= t) {
        return (h - t);
    } else {
        return (rb->size - t + h);
    }
}

uint16_t RingBuffer_GetFree(RingBuffer *rb) {
    // 留一个位置不写，用于区分空和满
    return (rb->size - RingBuffer_GetCount(rb) - 1);
}

uint16_t RingBuffer_Write(RingBuffer *rb, const uint8_t *data, uint16_t len) {
    uint16_t free_space = RingBuffer_GetFree(rb);
    if (len > free_space) {
        printf("RingBuffer_Write: no enough space\n");
        return 0; // 没有足够空间写入
    }

    if (len == 0) return 0;

    uint16_t first_part = rb->size - rb->head;

    if (len <= first_part) {
        // 不需要回绕，直接拷贝
        memcpy(&rb->buffer[rb->head], data, len);
    } else {
        // 需要回绕，分两段拷贝
        memcpy(&rb->buffer[rb->head], data, first_part);
        memcpy(&rb->buffer[0], &data[first_part], len - first_part);
    }

    rb->head = (rb->head + len) % rb->size;
    return len;
}

uint16_t RingBuffer_Read(RingBuffer *rb, uint8_t *dest, uint16_t len) {
    uint16_t count = RingBuffer_GetCount(rb);
    if (len > count) {
        len = count; // 没那么多数据
    }

    if (len == 0) return 0;

    uint16_t first_part = rb->size - rb->tail;

    if (len <= first_part) {
        // 数据连续，直接读取
        memcpy(dest, &rb->buffer[rb->tail], len);
    } else {
        // 数据回绕，分两段读取
        memcpy(dest, &rb->buffer[rb->tail], first_part);
        memcpy(&dest[first_part], &rb->buffer[0], len - first_part);
    }

    rb->tail = (rb->tail + len) % rb->size;
    return len;
}

uint16_t RingBuffer_Find_Char(RingBuffer *rb, uint8_t target_char) {
    uint16_t count = RingBuffer_GetCount(rb);
    
    // 如果水库是空的，直接收工
    if (count == 0) {
        return 0; 
    }

    // 从现在的读指针(tail)开始，顺藤摸瓜往后找
    for (uint16_t i = 0; i < count; i++) {
        // 【核心】：利用取余(%)完美解决在内存物理边界上的回绕问题
        uint16_t scan_index = (rb->tail + i) % rb->size; 
        
        if (rb->buffer[scan_index] == target_char) {
            return i + 1; 
        }
    }
    
    // 整个水库都扫遍了，没找到目标字符（说明半包，还在等）
    return 0; 
}
