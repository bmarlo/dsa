#include "marlo/queue.h"
#include "marlo/linked_list.h"

#include <stdlib.h>

struct queue_t {
    linked_list_t* values;
};

queue_t* queue_new()
{
    queue_t* queue = (queue_t*) malloc(sizeof(queue_t));
    if (queue == NULL) {
        return NULL;
    }

    linked_list_t* values = linked_list_new();
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

    return linked_list_push_back(queue->values, value);
}

const void* queue_pop(queue_t* queue)
{
    if (queue == NULL) {
        return NULL;
    }

    return linked_list_pop_front(queue->values);
}

int queue_is_empty(const queue_t* queue)
{
    return queue != NULL ? linked_list_is_empty(queue->values) : 0;
}

void queue_clear(queue_t* queue)
{
    if (queue != NULL) {
        linked_list_clear(queue->values);
    }
}

size_t queue_size(const queue_t* queue)
{
    return queue != NULL ? linked_list_size(queue->values) : 0;
}

void queue_release(queue_t* queue)
{
    if (queue != NULL) {
        linked_list_release(queue->values);
        free(queue);
    }
}
