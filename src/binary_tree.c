#include "marlo/binary_tree.h"
#include "marlo/queue.h"
#include "marlo/stack.h"

#include <stdlib.h>

struct tree_node_t {
    binary_tree_t* tree;
    const void* value;
    tree_node_t* parent;
    tree_node_t* left;
    tree_node_t* right;
};

struct binary_tree_t {
    tree_node_t* root;
    size_t size;
};

binary_tree_t* binary_tree_new()
{
    binary_tree_t* tree = (binary_tree_t*) malloc(sizeof(binary_tree_t));
    if (tree == NULL) {
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    return tree;
}

static tree_node_t* binary_tree_new_node(binary_tree_t* tree, tree_node_t* parent, const void* value)
{
    tree_node_t* node = (tree_node_t*) malloc(sizeof(tree_node_t));
    if (node == NULL) {
        return NULL;
    }

    node->tree = tree;
    node->value = value;
    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    return node;
}

tree_node_t* binary_tree_push(binary_tree_t* tree, const void* value)
{
    if (tree == NULL || value == NULL) {
        return NULL;
    }

    tree_node_t* node = NULL;
    if (tree->root == NULL) {
        node = binary_tree_new_node(tree, NULL, value);
        if (node == NULL) {
            return NULL;
        }

        tree->root = node;
    } else {
        queue_t* queue = queue_new(0);
        int error = queue_push(queue, tree->root);
        if (error == -1) {
            queue_release(queue);
            return NULL;
        }

        do {
            tree_node_t* iter = (tree_node_t*) queue_pop(queue);
            if (iter->left == NULL) {
                node = binary_tree_new_node(tree, iter, value);
                if (node == NULL) {
                    queue_release(queue);
                    return NULL;
                }

                iter->left = node;
                break;
            }

            error = queue_push(queue, iter->left);
            if (error == -1) {
                queue_release(queue);
                return NULL;
            }

            if (iter->right == NULL) {
                node = binary_tree_new_node(tree, iter, value);
                if (node == NULL) {
                    queue_release(queue);
                    return NULL;
                }

                iter->right = node;
                break;
            }

            error = queue_push(queue, iter->right);
            if (error == -1) {
                queue_release(queue);
                return NULL;
            }
        } while (!queue_is_empty(queue));

        queue_release(queue);
    }

    tree->size++;
    return node;
}

tree_node_t* binary_tree_push_left(tree_node_t* node, const void* value)
{
    if (node == NULL || node->left != NULL || value == NULL) {
        return NULL;
    }

    tree_node_t* child = binary_tree_new_node(node->tree, node, value);
    if (child == NULL) {
        return NULL;
    }

    node->left = child;
    node->tree->size++;
    return child;
}

tree_node_t* binary_tree_push_right(tree_node_t* node, const void* value)
{
    if (node == NULL || node->right != NULL || value == NULL) {
        return NULL;
    }

    tree_node_t* child = binary_tree_new_node(node->tree, node, value);
    if (child == NULL) {
        return NULL;
    }

    node->right = child;
    node->tree->size++;
    return child;
}

const void* binary_tree_value(const tree_node_t* node)
{
    return node != NULL ? node->value : NULL;
}

static int binary_tree_traverse_in_order(const binary_tree_t* tree, callback_t on_value)
{
    stack_t* stack = stack_new(0);
    if (stack == NULL) {
        return -1;
    }

    tree_node_t* iter = tree->root;
    while (iter != NULL || !stack_is_empty(stack)) {
        while (iter != NULL) {
            int error = stack_push(stack, iter);
            if (error == -1) {
                stack_release(stack);
                return -1;
            }

            iter = iter->left;
        }

        iter = (tree_node_t*) stack_pop(stack);
        on_value(iter->value);
        iter = iter->right;
    }

    stack_release(stack);
    return 0;
}

static int binary_tree_traverse_pre_order(const binary_tree_t* tree, callback_t on_value)
{
    stack_t* stack = stack_new(0);
    if (stack == NULL) {
        return -1;
    }

    tree_node_t* iter = tree->root;
    while (iter != NULL || !stack_is_empty(stack)) {
        while (iter != NULL) {
            on_value(iter->value);
            int error = stack_push(stack, iter);
            if (error == -1) {
                stack_release(stack);
                return -1;
            }

            iter = iter->left;
        }

        iter = (tree_node_t*) stack_pop(stack);
        iter = iter->right;
    }

    stack_release(stack);
    return 0;
}

static int binary_tree_traverse_post_order(const binary_tree_t* tree, callback_t on_value)
{
    stack_t* stack = stack_new(0);
    if (stack == NULL) {
        return -1;
    }

    tree_node_t* last = NULL;
    tree_node_t* iter = tree->root;
    while (iter != NULL || !stack_is_empty(stack)) {
        while (iter != NULL) {
            int error = stack_push(stack, iter);
            if (error == -1) {
                stack_release(stack);
                return -1;
            }

            iter = iter->left;
        }

        iter = (tree_node_t*) stack_peek(stack);
        if (iter->right != NULL && iter->right != last) {
            iter = iter->right;
        } else {
            last = iter;
            on_value(iter->value);
            stack_pop(stack);
            if (stack_is_empty(stack)) {
                break;
            }

            iter = (tree_node_t*) stack_peek(stack);
            iter = iter->right != NULL && iter->right != last ? iter->right : NULL;
        }
    }

    stack_release(stack);
    return 0;
}

static int binary_tree_traverse_level_order(const binary_tree_t* tree, callback_t on_value)
{
    queue_t* queue = queue_new(0);
    if (queue == NULL) {
        return -1;
    }

    if (tree->root != NULL) {
        int error = queue_push(queue, tree->root);
        if (error == -1) {
            queue_release(queue);
            return -1;
        }
    }

    while (!queue_is_empty(queue)) {
        tree_node_t* node = (tree_node_t*) queue_pop(queue);
        on_value(node->value);

        if (node->left != NULL) {
            int error = queue_push(queue, node->left);
            if (error == -1) {
                queue_release(queue);
                return -1;
            }
        }

        if (node->right != NULL) {
            int error = queue_push(queue, node->right);
            if (error == -1) {
                queue_release(queue);
                return -1;
            }
        }
    }

    queue_release(queue);
    return 0;
}

int binary_tree_traverse(const binary_tree_t* tree, int order, callback_t on_value)
{
    if (tree == NULL || on_value == NULL) {
        return -1;
    }

    switch (order) {
    case BINARY_TREE_IN_ORDER:
        return binary_tree_traverse_in_order(tree, on_value);
    case BINARY_TREE_PRE_ORDER:
        return binary_tree_traverse_pre_order(tree, on_value);
    case BINARY_TREE_POST_ORDER:
        return binary_tree_traverse_post_order(tree, on_value);
    case BINARY_TREE_LEVEL_ORDER:
        return binary_tree_traverse_level_order(tree, on_value);
    default:
        break;
    }

    return -1;
}

int binary_tree_is_empty(const binary_tree_t* tree)
{
    return binary_tree_size(tree) == 0;
}

void binary_tree_clear(binary_tree_t* tree)
{
    if (tree != NULL) {
        tree_node_t* iter = tree->root;
        while (iter != NULL) {
            if (iter->left != NULL) {
                iter = iter->left;
            } else if (iter->right != NULL) {
                iter = iter->right;
            } else {
                tree_node_t* prev = iter->parent;
                if (prev != NULL) {
                    if (prev->left == iter) {
                        prev->left = NULL;
                    } else {
                        prev->right = NULL;
                    }
                }

                free(iter);
                iter = prev;
            }
        }

        tree->root = NULL;
        tree->size = 0;
    }
}

size_t binary_tree_size(const binary_tree_t* tree)
{
    return tree != NULL ? tree->size : 0;
}

void binary_tree_release(binary_tree_t* tree)
{
    binary_tree_clear(tree);
    free(tree);
}
