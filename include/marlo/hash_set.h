#pragma once

#include <stddef.h>

/**
 * Opaque hash set type.
 */
typedef struct hash_set_t hash_set_t;

/**
 * Hash set iterator type.
 */
typedef struct hash_set_iterator_t {
    const hash_set_t* set;
    size_t pos;
    const void* node;
} hash_set_iterator_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new hash set with the given capacity.
 * Returns the new set on success or `NULL` on error.
 * The set must be deallocated with `hash_set_release()`.
 */
hash_set_t* hash_set_new(size_t capacity);

/**
 * Adds an element into the set.
 * `value` cannot be `NULL`.
 * If the value already exists within the set, it's updated.
 * Returns 0 on success or -1 on error.
 */
int hash_set_push(hash_set_t* set, const void* value);

/**
 * Whether the set is empty.
 * Returns 1 if the set is empty, 0 otherwise.
 */
int hash_set_is_empty(const hash_set_t* set);

/**
 * Whether the set contains the given value.
 * Returns 1 if the set contains the value, 0 otherwise.
 */
int hash_set_contains(const hash_set_t* set, const void* value);

/**
 * Returns an iterator pointing to the beginning of the given set or a
 * `NULL`ed/zero struct on error (empty set).
 */
hash_set_iterator_t hash_set_begin(const hash_set_t* set);

/**
 * Whether the given iterator is valid.
 * Returns 1 if the iterator is valid, 0 otherwise.
 */
int hash_set_is_valid(hash_set_iterator_t iter);

/**
 * Returns an iterator pointing to the value next/after the given iterator or a
 * `NULL`ed/zero struct on error (no next).
 */
hash_set_iterator_t hash_set_next(hash_set_iterator_t iter);

/**
 * Returns the value at the given iterator or `NULL` on error (bad iterator).
 */
const void* hash_set_value(hash_set_iterator_t iter);

/**
 * Removes an element from the set.
 * Does nothing if the value doesn't exist within the set.
 */
void hash_set_remove(hash_set_t* set, const void* value);

/**
 * Removes all elements from the set.
 * The set's capacity is not changed.
 */
void hash_set_clear(hash_set_t* set);

/**
 * Returns the number of elements in the set.
 */
size_t hash_set_size(const hash_set_t* set);

/**
 * Returns the capacity of the set.
 */
size_t hash_set_capacity(const hash_set_t* set);

/**
 * Returns the load factor (aka `size / capacity`) of the set.
 */
float hash_set_load_factor(const hash_set_t* set);

/**
 * Deallocates the given set.
 * `set` must not be reused.
 */
void hash_set_release(hash_set_t* set);

#ifdef __cplusplus
}
#endif
