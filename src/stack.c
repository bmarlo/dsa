#include "marlo/stack.h"
#include "marlo/vector.h"

#include <stdlib.h>

struct stack_t {
    vector_t* values;
};

stack_t* stack_new()
{
    stack_t* stack = (stack_t*) malloc(sizeof(stack_t));
    if (stack == NULL) {
        return NULL;
    }

    vector_t* values = vector_new(0);
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

    return vector_push(stack->values, value);
}

const void* stack_pop(stack_t* stack)
{
    if (stack == NULL) {
        return NULL;
    }

    return vector_pop(stack->values);
}

int stack_is_empty(const stack_t* stack)
{
    return stack != NULL ? vector_is_empty(stack->values) : 0;
}

void stack_clear(stack_t* stack)
{
    if (stack != NULL) {
        vector_clear(stack->values);
    }
}

size_t stack_size(const stack_t* stack)
{
    return stack != NULL ? vector_size(stack->values) : 0;
}

void stack_release(stack_t* stack)
{
    if (stack != NULL) {
        vector_release(stack->values);
        free(stack);
    }
}
