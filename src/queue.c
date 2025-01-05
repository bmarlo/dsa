#include "marlo/queue.h"
#include "marlo/deque.h"

#include <stdlib.h>

struct queue_t {
    deque_t* values;
};

queue_t* queue_new(size_t capacity)
{
    queue_t* queue = (queue_t*) malloc(sizeof(queue_t));
    if (queue == NULL) {
        return NULL;
    }

    deque_t* values = deque_new(capacity);
    if (values == NULL) {
        free(queue);
        return NULL;
    }

    queue->values = values;
    return queue;
}

int queue_push(queue_t* queue, const void* value)
{
    if (queue == NULL) {
        return -1;
    }

    return deque_push_back(queue->values, value);
}

const void* queue_pop(queue_t* queue)
{
    if (queue == NULL) {
        return NULL;
    }

    return deque_pop_front(queue->values);
}

const void* queue_peek(const queue_t* queue)
{
    if (queue == NULL) {
        return NULL;
    }

    return deque_front(queue->values);
}

int queue_is_empty(const queue_t* queue)
{
    return queue_size(queue) == 0;
}

void queue_clear(queue_t* queue)
{
    if (queue != NULL) {
        deque_clear(queue->values);
    }
}

size_t queue_size(const queue_t* queue)
{
    return queue != NULL ? deque_size(queue->values) : 0;
}

void queue_release(queue_t* queue)
{
    if (queue != NULL) {
        deque_release(queue->values);
        free(queue);
    }
}
