#include "marlo/stack.h"
#include "marlo/deque.h"

#include <stdlib.h>

struct stack_t {
    deque_t* values;
};

stack_t* stack_new(size_t capacity)
{
    stack_t* stack = (stack_t*) malloc(sizeof(stack_t));
    if (stack == NULL) {
        return NULL;
    }

    deque_t* values = deque_new(capacity);
    if (values == NULL) {
        free(stack);
        return NULL;
    }

    stack->values = values;
    return stack;
}

int stack_push(stack_t* stack, const void* value)
{
    if (stack == NULL) {
        return -1;
    }

    return deque_push_back(stack->values, value);
}

const void* stack_pop(stack_t* stack)
{
    if (stack == NULL) {
        return NULL;
    }

    return deque_pop_back(stack->values);
}

const void* stack_peek(const stack_t* stack)
{
    if (stack == NULL) {
        return NULL;
    }

    return deque_back(stack->values);
}

int stack_is_empty(const stack_t* stack)
{
    return stack_size(stack) == 0;
}

void stack_clear(stack_t* stack)
{
    if (stack != NULL) {
        deque_clear(stack->values);
    }
}

size_t stack_size(const stack_t* stack)
{
    return stack != NULL ? deque_size(stack->values) : 0;
}

void stack_release(stack_t* stack)
{
    if (stack != NULL) {
        deque_release(stack->values);
        free(stack);
    }
}
