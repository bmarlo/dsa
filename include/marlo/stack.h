#pragma once

#include <stddef.h>

/**
 * Opaque stack type.
 */
typedef struct stack_t stack_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new stack (aka LIFO queue).
 * Returns the stack on success or `NULL` on error.
 * The stack must be deallocated with `stack_release()`.
 */
stack_t* stack_new();

/**
 * Pushes an element to the stack.
 * `value` cannot be `NULL`.
 * Returns 0 on success or -1 on error.
 */
int stack_push(stack_t* stack, const void* value);

/**
 * Pops an element from the stack.
 * Returns `NULL` on error (empty stack).
 */
const void* stack_pop(stack_t* stack);

/**
 * Whether the stack is empty.
 * Returns 1 if the stack is empty, 0 otherwise.
 */
int stack_is_empty(const stack_t* stack);

/**
 * Removes all elements from the stack, setting its size to zero.
 * The stack's capacity is not changed.
 */
void stack_clear(stack_t* stack);

/**
 * Returns the number of elements in the stack.
 */
size_t stack_size(const stack_t* stack);

/**
 * Deallocates the given stack.
 * `stack` must not be reused.
 */
void stack_release(stack_t* stack);

#ifdef __cplusplus
}
#endif
