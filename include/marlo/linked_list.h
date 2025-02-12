#pragma once

#include <stddef.h>

/**
 * Opaque linked list type.
 */
typedef struct linked_list_t linked_list_t;

/**
 * Opaque list node type for efficient iteration.
 */
typedef struct list_node_t list_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new linked list.
 * Returns the new list on success or `NULL` on error.
 * The list must be deallocated with `linked_list_release()`.
 */
linked_list_t* linked_list_new();

/**
 * Adds an element to the end of the list.
 * `value` cannot be `NULL.`
 * Returns 0 on success or -1 on error.
 */
int linked_list_push_back(linked_list_t* list, const void* value);

/**
 * Adds an element to the beginning of the list.
 * `value` cannot be `NULL.`
 * Returns 0 on success or -1 on error.
 */
int linked_list_push_front(linked_list_t* list, const void* value);

/**
 * Removes and returns the last element from the list.
 * Returns `NULL` on error (empty list).
 */
const void* linked_list_pop_back(linked_list_t* list);

/**
 * Removes and returns the first element from the list.
 * Returns `NULL` on error (empty list).
 */
const void* linked_list_pop_front(linked_list_t* list);

/**
 * Whether the list is empty.
 * Returns 1 if the list is empty, 0 otherwise.
 */
int linked_list_is_empty(const linked_list_t* list);

/**
 * Returns the last node of the list or `NULL` on error (empty list).
 */
const list_node_t* linked_list_back(const linked_list_t* list);

/**
 * Returns the first node of the list or `NULL` on error (empty list).
 */
const list_node_t* linked_list_front(const linked_list_t* list);

/**
 * Returns the node after the given node, or `NULL` on error (no next).
 */
const list_node_t* linked_list_next(const list_node_t* node);

/**
 * Returns the node before the given node, or `NULL` on error (no previous).
 */
const list_node_t* linked_list_previous(const list_node_t* node);

/**
 * Returns the value of the given node.
 */
const void* linked_list_value(const list_node_t* node);

/**
 * Returns the number of elements in the given list.
 */
size_t linked_list_size(const linked_list_t* list);

/**
 * Removes the given element from the list.
 * Does nothing if the value doesn't exist within the list.
 */
void linked_list_remove(linked_list_t* list, const void* value);

/**
 * Removes all elements from the list.
 * Previously returned nodes must not be reused.
 */
void linked_list_clear(linked_list_t* list);

/**
 * Deallocates the given list.
 * `list` must not be reused.
 */
void linked_list_release(linked_list_t* list);

#ifdef __cplusplus
}
#endif
