#pragma once

#include <stddef.h>

/**
 * Opaque hash table type.
 */
typedef struct hash_table_t hash_table_t;

/**
 * Pointer/address mode.
 * Keys are treated as integer address values.
 */
#define HASH_TABLE_ADDRESS 1

/**
 * String mode.
 * Keys must be null-terminated strings.
 */
#define HASH_TABLE_STRING 2

/**
 * Hash table iterator type.
 */
typedef struct hash_table_iterator_t {
    const hash_table_t* table;
    size_t pos;
    const void* node;
} hash_table_iterator_t;

/**
 * Hash table key-value pair.
 */
typedef struct hash_table_item_t {
    const void* key;
    const void* value;
} hash_table_item_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new hash table with the given capacity and mode of operation.
 * `mode` must be either `HASH_TABLE_ADDRESS` or `HASH_TABLE_STRING`.
 * Returns the table on success or `NULL` on error.
 * The table must be deallocated with `hash_table_release()`.
 */
hash_table_t* hash_table_new(size_t capacity, int mode);

/**
 * Returns the mode of the table or -1 on error (`NULL` table).
 */
int hash_table_mode(const hash_table_t* table);

/**
 * Whether the table is in address mode.
 * Returns 1 if the table's mode is `HASH_TABLE_ADDRESS`, 0 otherwise.
 */
int hash_table_is_address(const hash_table_t* table);

/**
 * Whether the table is in string mode.
 * Returns 1 if the table's mode is `HASH_TABLE_STRING`, 0 otherwise.
 */
int hash_table_is_string(const hash_table_t* table);

/**
 * Adds a key-value pair into the given table.
 * `key` can only be `NULL` in `HASH_TABLE_ADDRESS` mode.
 * `value` cannot be `NULL`.
 * If the value already exists at the given key, it's updated.
 * Returns 0 on success or -1 on error.
 */
int hash_table_push(hash_table_t* table, const void* key, const void* value);

/**
 * Returns the value at the given key or `NULL` on error (not found).
 */
const void* hash_table_at(const hash_table_t* table, const void* key);

/**
 * Whether the table is empty.
 * Returns 1 if the table is empty, 0 otherwise.
 */
int hash_table_is_empty(const hash_table_t* table);

/**
 * Whether the table contains a value for the given `key`.
 * Returns 1 if the table contains a value for the key, 0 otherwise.
 */
int hash_table_contains(const hash_table_t* table, const void* key);

/**
 * Returns an iterator pointing to the beginning of the given table or a
 * `NULL`ed/zero struct on error (empty table).
 */
hash_table_iterator_t hash_table_begin(const hash_table_t* table);

/**
 * Whether the given iterator is valid.
 * Returns 1 if the iterator is valid, 0 otherwise.
 */
int hash_table_is_valid(hash_table_iterator_t iter);

/**
 * Returns an iterator pointing to the item next/after the given iterator or a
 * `NULL`ed/zero struct on error (no next).
 */
hash_table_iterator_t hash_table_next(hash_table_iterator_t iter);

/**
 * Returns the item/key-value pair at the given iterator or a `NULL`ed/zero
 * struct on error (bad iterator).
 * The item is valid if its `value` field is not `NULL`.
 */
hash_table_item_t hash_table_item(hash_table_iterator_t iter);

/**
 * Removes a key-value pair from the table.
 * Does nothing if the key doesn't exist within the table.
 */
void hash_table_remove(hash_table_t* table, const void* key);

/**
 * Removes all key-value pairs from the given table.
 * The table's capacity is not changed.
 */
void hash_table_clear(hash_table_t* table);

/**
 * Returns the number of key-value pairs in the table.
 */
size_t hash_table_size(const hash_table_t* table);

/**
 * Returns the capacity of the table.
 */
size_t hash_table_capacity(const hash_table_t* table);

/**
 * Returns the load factor (aka `size / capacity`) of the table.
 */
float hash_table_load_factor(const hash_table_t* table);

/**
 * Deallocates the given table.
 * `table` must not be reused.
 */
void hash_table_release(hash_table_t* table);

#ifdef __cplusplus
}
#endif
