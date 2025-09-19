#ifndef __DEQUEUE_H__
#define __DEQUEUE_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* voidPtr_t;
typedef voidPtr_t data_t;
typedef uint16_t dequeueIndex_t;
typedef uint32_t dqConfig_t;
#define DQ_INVALID_VALUE  ((data_t)0xFFFF)  // Giá trị đặc biệt khi pop từ deque rỗng

enum DEQUEUE_ENUM_ORDER_BITMASK {
    HEAD_OVERWRITE_WHEN_FULL = 1 << 0,  // 0x01: overwrite oldest element
    TAIL_OVERWRITE_WHEN_FULL = 1 << 1,  // 0x02: overwrite newest element
};

#define pv2type(pv, type)   ((type*)(pv))
#define pvnew(type)         malloc(sizeof(type));
#define pvfree(pv)          if(pv) {free(pv); (pv) = NULL;}

/**
 * @brief Deque (Double-Ended Queue) control structure.
 */
typedef struct dequeue_t {
    dequeueIndex_t  dqHead;        // Index of the first element
    dequeueIndex_t  dqTail;        // Index of the next available slot at the end
    dequeueIndex_t  dqSize;        // Maximum capacity of the buffer
    dequeueIndex_t  dqCount;       // Current number of elements in the deque
    data_t *        dequeueBuff;          // Pointer to the data buffer
    dqConfig_t      conf;              // Configuration flags
} dequeue_t;

void createDequeue(dequeue_t **dq, dequeueIndex_t size);
void destroyDequeue(dequeue_t **dq);
void dqSetConfig(dequeue_t *dq, dqConfig_t config);
uint8_t dqIsEmpty(dequeue_t *dq);
uint8_t dqIsFull(dequeue_t *dq);
dequeueIndex_t dqCount(dequeue_t *dq);
void dqPushback(dequeue_t *dq, data_t val);
data_t dqPop(dequeue_t *dq);
void dqPush(dequeue_t *dq, data_t val);
data_t dqPopback(dequeue_t *dq);
data_t dqAt(dequeue_t *dq, dequeueIndex_t i);
data_t *dqPtrAt(dequeue_t *dq, dequeueIndex_t i);

#ifdef __cplusplus
}
#endif

#endif