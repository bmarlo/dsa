#pragma once

#include <stddef.h>

/**
 * Opaque deque type.
 */
typedef struct deque_t deque_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new deque (aka double-ended queue) with the given capacity.
 * Returns the deque on success or `NULL` on error.
 * The deque must be deallocated with `deque_release()`.
 */
deque_t* deque_new(size_t capacity);

/**
 * Adds an element to the back of the deque.
 * `value` cannot be `NULL`.
 * Returns 0 on success or -1 on error.
 */
int deque_push_back(deque_t* deque, const void* value);

/**
 * Adds an element to the front of the deque.
 * `value` cannot be `NULL`.
 * Returns 0 on success or -1 on error.
 */
int deque_push_front(deque_t* deque, const void* value);

/**
 * Removes and returns the element at the back at the deque.
 * Returns `NULL` on error (empty deque).
 */
const void* deque_pop_back(deque_t* deque);

/**
 * Removes and returns the element at the front at the deque.
 * Returns `NULL` on error (empty deque).
 */
const void* deque_pop_front(deque_t* deque);

/**
 * Returns the element at the back of the deque, the same element returned by
 * `deque_pop_back()`, without removing it.
 * Returns `NULL` on error (empty deque).
 */
const void* deque_back(deque_t* deque);

/**
 * Returns the element at the front of the deque, the same element returned by
 * `deque_pop_front()`, without removing it.
 * Returns `NULL` on error (empty deque).
 */
const void* deque_front(deque_t* deque);

/**
 * Whether the deque is empty.
 * Returns 1 if the deque is empty, 0 otherwise.
 */
int deque_is_empty(const deque_t* deque);

/**
 * Removes all elements from the deque.
 * The deque's capacity is not changed.
 */
void deque_clear(deque_t* deque);

/**
 * Returns the number of elements in the deque.
 */
size_t deque_size(const deque_t* deque);

/**
 * Returns the capacity of the deque.
 */
size_t deque_capacity(const deque_t* deque);

/**
 * Deallocates the given deque.
 * `deque` must not be reused.
 */
void deque_release(deque_t* deque);

#ifdef __cplusplus
}
#endif
