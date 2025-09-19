#include "dequeue.h"

#include "../../include/global.h"




void createDequeue(dequeue_t **dq, dequeueIndex_t size) {
    __entry("createDequeue(%p, %d)", dq, size);
    if (!dq || size == 0) return;

    *dq = (dequeue_t *)malloc(sizeof(dequeue_t));
    if (!(*dq)) return;

    (*dq)->dequeueBuff = (data_t *)malloc(size * sizeof(data_t));
    if (!(*dq)->dequeueBuff) {
        free(*dq);
        *dq = NULL;
        return;
    }

    (*dq)->dqHead = 0;
    (*dq)->dqTail = 0;
    (*dq)->dqSize = size;
    (*dq)->dqCount = 0;
    (*dq)->conf = 0;
    __exit("createDequeue()");
}

void destroyDequeue(dequeue_t **dq) {
    if (!dq || !*dq) return;
    free((*dq)->dequeueBuff);
    free(*dq);
    *dq = NULL;
}

void dqSetConfig(dequeue_t *dq, dqConfig_t config) {
    if (dq) dq->conf = config;
}

uint8_t dqIsEmpty(dequeue_t *dq) {
    return (!dq || dq->dqCount == 0);
}

uint8_t dqIsFull(dequeue_t *dq) {
    return (dq && dq->dqCount == dq->dqSize);
}

dequeueIndex_t dqCount(dequeue_t *dq) {
    return dq ? dq->dqCount : 0;
}

void dqPushback(dequeue_t *dq, data_t val) {
    if (!dq) return;

    if (dqIsFull(dq)) {
        if (dq->conf & HEAD_OVERWRITE_WHEN_FULL) {
            dq->dqHead = (dq->dqHead + 1) % dq->dqSize; // bỏ phần tử cũ nhất
            dq->dqCount--;
        } else {
            return; // Không cho push nếu full
        }
    }

    dq->dequeueBuff[dq->dqTail] = val;
    dq->dqTail = (dq->dqTail + 1) % dq->dqSize;
    dq->dqCount++;
}

data_t dqPop(dequeue_t *dq) {
    if (!dq || dqIsEmpty(dq)) {
        return DQ_INVALID_VALUE;
    }

    data_t val = dq->dequeueBuff[dq->dqHead];
    dq->dqHead = (dq->dqHead + 1) % dq->dqSize;
    dq->dqCount--;
    return val;
}

void dqPush(dequeue_t *dq, data_t val) {
    if (!dq) return;

    if (dqIsFull(dq)) {
        if (dq->conf & TAIL_OVERWRITE_WHEN_FULL) {
            dq->dqTail = (dq->dqTail == 0) ? (dq->dqSize - 1) : (dq->dqTail - 1);
            dq->dqCount--;
        } else {
            return;
        }
    }

    dq->dqHead = (dq->dqHead == 0) ? (dq->dqSize - 1) : (dq->dqHead - 1);
    dq->dequeueBuff[dq->dqHead] = val;
    dq->dqCount++;
}

data_t dqPopback(dequeue_t *dq) {
    if (!dq || dqIsEmpty(dq)) {
        return DQ_INVALID_VALUE;
    }

    dq->dqTail = (dq->dqTail == 0) ? (dq->dqSize - 1) : (dq->dqTail - 1);
    data_t val = dq->dequeueBuff[dq->dqTail];
    dq->dqCount--;
    return val;
}

data_t dqAt(dequeue_t *dq, dequeueIndex_t i) {
    if (!dq || !dq->dequeueBuff || dq->dqCount == 0) {
        return NULL;
    }
    dequeueIndex_t effective_index = i % dq->dqCount;
    dequeueIndex_t actual_index = (dq->dqHead + effective_index) % dq->dqSize;
    return dq->dequeueBuff[actual_index];
}

data_t *dqPtrAt(dequeue_t *dq, dequeueIndex_t i) {
    if (!dq || !dq->dequeueBuff || dq->dqCount == 0) {
        return NULL;
    }
    dequeueIndex_t effective_index = i % dq->dqCount;
    dequeueIndex_t actual_index = (dq->dqHead + effective_index) % dq->dqSize;
    return &dq->dequeueBuff[actual_index];
}

