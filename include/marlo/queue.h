#pragma once

#include <stddef.h>

/**
 * Opaque queue type.
 */
typedef struct queue_t queue_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new FIFO queue with the given capacity.
 * Returns the new queue on success or `NULL` on error.
 * The queue must be deallocated with `queue_release()`.
 */
queue_t* queue_new(size_t capacity);

/**
 * Adds an element to the queue.
 * `value` cannot be `NULL`.
 * Returns 0 on success or -1 on error.
 */
int queue_push(queue_t* queue, const void* value);

/**
 * Removes and returns the first element from the queue.
 * Returns `NULL` on error (empty queue).
 */
const void* queue_pop(queue_t* queue);

/**
 * Returns the first element of the queue without removing it.
 * Returns `NULL` on error (empty queue).
 */
const void* queue_peek(const queue_t* queue);

/**
 * Whether the queue is empty.
 * Returns 1 if the queue is empty, 0 otherwise.
 */
int queue_is_empty(const queue_t* queue);

/**
 * Removes all elements from the queue.
 * The queue's capacity is not changed.
 */
void queue_clear(queue_t* queue);

/**
 * Returns the number of elements in the queue.
 */
size_t queue_size(const queue_t* queue);

/**
 * Returns the capacity of the queue.
 */
size_t queue_capacity(const queue_t* queue);

/**
 * Deallocates the given queue.
 * `queue` must not be reused.
 */
void queue_release(queue_t* queue);

#ifdef __cplusplus
}
#endif
