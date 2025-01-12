#pragma once

#include "marlo/sorting.h"
#include <stddef.h>

/**
 * Opaque vector type.
 */
typedef struct vector_t vector_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new vector with the given capacity.
 * Returns the new vector on success or `NULL` on error.
 * The vector must be deallocated with `vector_release()`.
 */
vector_t* vector_new(size_t capacity);

/**
 * Adds an element to the end of the vector.
 * `value` cannot be `NULL`.
 * Returns 0 on success or -1 on error.
 */
int vector_push(vector_t* vector, const void* value);

/**
 * Removes and returns the last element from the vector.
 * Returns `NULL` on error (empty vector).
 */
const void* vector_pop(vector_t* vector);

/**
 * Returns the element at the given index or `NULL` on error (out of bounds).
 */
const void* vector_at(const vector_t* vector, size_t pos);

/**
 * Whether the vector is empty.
 * Returns 1 if the vector is empty, 0 otherwise.
 */
int vector_is_empty(const vector_t* vector);

/**
 * Removes the element at the given position from the vector, shifting all
 * elements to the right of it by one position to the left.
 */
void vector_remove(vector_t* vector, size_t pos);

/**
 * Removes all elements from the vector.
 * The vector's capacity is not changed.
 */
void vector_clear(vector_t* vector);

/**
 * Returns the number of elements in the vector.
 */
size_t vector_size(const vector_t* vector);

/**
 * Returns the capacity of the vector.
 */
size_t vector_capacity(const vector_t* vector);

/**
 * Sorts the vector using the given compare function.
 * Returns 0 on success or -1 on error (invalid arguments).
 */
int vector_sort(vector_t* vector, compare_t compare);

/**
 * Deallocates the given vector.
 * `vector` must not be reused.
 */
void vector_release(vector_t* vector);

#ifdef __cplusplus
}
#endif
