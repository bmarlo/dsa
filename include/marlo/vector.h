#pragma once

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
 * Returns the vector on success or `NULL` on error.
 * The vector must be deallocated with `vector_release()`.
 */
vector_t* vector_new(size_t capacity);

/**
 * Pushes a value to the back of the given vector.
 * Returns 0 on success or -1 on error.
 */
int vector_push(vector_t* vector, const void* value);

/**
 * Returns the value at the given index or `NULL` on error (out of bounds).
 */
const void* vector_at(const vector_t* vector, size_t pos);

/**
 * Whether the vector is empty.
 * Returns 1 if the vector is empty, 0 otherwise.
 */
int vector_is_empty(const vector_t* vector);

/**
 * Removes the value at the given position from the vector, shifting all values
 * to the right of it by one position to the left.
 */
void vector_remove(vector_t* vector, size_t pos);

/**
 * Removes all values from the given vector.
 * The vector's capacity is not changed.
 */
void vector_clear(vector_t* vector);

/**
 * Returns the number of values in the vector.
 */
size_t vector_size(const vector_t* vector);

/**
 * Returns the capacity of the vector.
 */
size_t vector_capacity(const vector_t* vector);

/**
 * Deallocates the given vector.
 * `vector` must not be reused.
 */
void vector_release(vector_t* vector);

#ifdef __cplusplus
}
#endif
