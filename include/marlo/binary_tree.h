#pragma once

#include <stddef.h>

/**
 * Opaque binary tree type.
 */
typedef struct binary_tree_t binary_tree_t;

/**
 * Opaque tree node type for efficient access.
 */
typedef struct tree_node_t tree_node_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a new binary tree.
 * Returns the new tree on success or `NULL` on error.
 * The tree must be deallocated with `binary_tree_release()`.
 */
binary_tree_t* binary_tree_new();

/**
 * Adds an element to the tree in level order.
 * `value` cannot be `NULL`.
 * Returns the new node on success or `NULL` on error.
 */
tree_node_t* binary_tree_push(binary_tree_t* tree, const void* value);

/**
 * Adds an element as the left child of the given node.
 * `value` cannot be `NULL`.
 * Returns the new node on success or `NULL` on error.
 */
tree_node_t* binary_tree_push_left(tree_node_t* node, const void* value);

/**
 * Adds an element as the right child of the given node.
 * `value` cannot be `NULL`.
 * Returns the new node on success or `NULL` on error.
 */
tree_node_t* binary_tree_push_right(tree_node_t* node, const void* value);

/**
 * Returns the value of the given node.
 */
const void* binary_tree_value(const tree_node_t* node);

/**
 * Whether the tree is empty.
 * Returns 1 if the tree is empty, 0 otherwise.
 */
int binary_tree_is_empty(const binary_tree_t* tree);

/**
 * Removes all elements/nodes from the tree.
 * Previously returned nodes must not be reused.
 */
void binary_tree_clear(binary_tree_t* tree);

/**
 * Returns the number of elements in the tree.
 */
size_t binary_tree_size(const binary_tree_t* tree);

/**
 * Deallocates the given tree.
 * `tree` must not be reused.
 */
void binary_tree_release(binary_tree_t* tree);

#ifdef __cplusplus
}
#endif
