#pragma once

#include <stddef.h>

/**
 * Opaque string type.
 */
typedef struct string_t string_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new string with the given capacity.
 * Returns the new string on success or `NULL` on error.
 * The string must be deallocated with `string_release()`.
 */
string_t* string_new(size_t capacity);

/**
 * Appends a char to the string.
 * Returns 0 on success or -1 on error.
 */
int string_push(string_t* string, char c);

/**
 * Appends a substring.
 * Returns 0 on success or -1 on error.
 */
int string_append(string_t* string, const char* str);

/**
 * Clears the string.
 * The string's capacity is not changed.
 */
void string_clear(string_t* string);

/**
 * Whether the string is empty.
 * Returns 1 if the string is empty, 0 otherwise.
 */
int string_is_empty(const string_t* string);

/**
 * Returns the string data, always non-null and null-terminated.
 */
const char* string_value(const string_t* string);

/**
 * Returns the string size.
 */
size_t string_size(const string_t* string);

/**
 * Returns the capacity of the string.
 */
size_t string_capacity(const string_t* string);

/**
 * Deallocates the given string.
 * `string` must not be reused.
 */
void string_release(string_t* string);

#ifdef __cplusplus
}
#endif
