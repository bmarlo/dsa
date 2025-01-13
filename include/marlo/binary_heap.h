#pragma once

#include "marlo/sorting.h"
#include <stddef.h>

/**
 * Opaque binary heap type.
 */
typedef struct binary_heap_t binary_heap_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new binary heap with the given capacity and compare function.
 * Returns the new heap on success or `NULL` on error.
 * The heap must be deallocated with `binary_heap_release()`.
 */
binary_heap_t* binary_heap_new(size_t capacity, compare_t compare);

/**
 * Adds an element to the heap.
 * `value` cannot be `NULL`.
 * Returns 0 on success or -1 on error.
 */
int binary_heap_push(binary_heap_t* heap, const void* value);

/**
 * Removes and returns the root element from the heap.
 * Returns `NULL` on error (empty heap).
 */
const void* binary_heap_pop(binary_heap_t* heap);

/**
 * Returns the element at the root of the heap without removing it.
 * Returns `NULL` on error (empty heap).
 */
const void* binary_heap_peek(const binary_heap_t* heap);

/**
 * Whether the heap is empty.
 * Returns 1 if the heap is empty, 0 otherwise.
 */
int binary_heap_is_empty(const binary_heap_t* heap);

/**
 * Removes all elements from the heap.
 * The heap's capacity is not changed.
 */
void binary_heap_clear(binary_heap_t* heap);

/**
 * Returns the number of elements in the heap.
 */
size_t binary_heap_size(const binary_heap_t* heap);

/**
 * Returns the capacity of the heap.
 */
size_t binary_heap_capacity(const binary_heap_t* heap);

/**
 * Deallocates the given heap.
 * `heap` must not be reused.
 */
void binary_heap_release(binary_heap_t* heap);

#ifdef __cplusplus
}
#endif
