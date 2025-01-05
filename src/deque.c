#include "marlo/deque.h"

#include <stdlib.h>

#define RESIZE_FACTOR 2

struct deque_t {
    const void** values;
    size_t capacity;
    size_t size;
    size_t head;
    size_t tail;
};

deque_t* deque_new(size_t capacity)
{
    deque_t* deque = (deque_t*) malloc(sizeof(deque_t));
    if (deque == NULL) {
        return NULL;
    }

    deque->values = NULL;
    deque->capacity = capacity;
    deque->size = 0;
    deque->head = 0;
    deque->tail = 0;

    if (capacity > 0) {
        const void** values = (const void**) malloc(capacity * sizeof(void*));
        if (values == NULL) {
            free(deque);
            return NULL;
        }

        for (size_t i = 0; i < capacity; i++) {
            values[i] = NULL;
        }

        deque->values = values;
    }

    return deque;
}

static int deque_resize(deque_t* deque)
{
    size_t new_capacity = deque->capacity > 0 ? deque->capacity * RESIZE_FACTOR : 1;
    const void** new_values = (const void**) malloc(new_capacity * sizeof(void*));
    if (new_values == NULL) {
        return -1;
    }

    size_t k = 0;
    for (size_t i = deque->head; i < deque->capacity; i++) {
        new_values[k++] = deque->values[i];
    }

    for (size_t i = 0; i < deque->tail; i++) {
        new_values[k++] = deque->values[i];
    }

    while (k < new_capacity) {
        new_values[k++] = NULL;
    }

    free(deque->values);
    deque->values = new_values;
    deque->capacity = new_capacity;
    deque->head = 0;
    deque->tail = deque->size;
    return 0;
}

int deque_push_back(deque_t* deque, const void* value)
{
    if (deque == NULL || value == NULL) {
        return -1;
    }

    if (deque->size == deque->capacity) {
        int error = deque_resize(deque);
        if (error == -1) {
            return -1;
        }
    }

    deque->values[deque->tail] = value;
    deque->tail = deque->tail == deque->capacity - 1 ? 0 : deque->tail + 1;
    deque->size++;
    return 0;
}

int deque_push_front(deque_t* deque, const void* value)
{
    if (deque == NULL || value == NULL) {
        return -1;
    }

    if (deque->size == deque->capacity) {
        int error = deque_resize(deque);
        if (error == -1) {
            return -1;
        }
    }

    deque->head = deque->head == 0 ? deque->capacity - 1 : deque->head - 1;
    deque->values[deque->head] = value;
    deque->size++;
    return 0;
}

const void* deque_pop_back(deque_t* deque)
{
    if (deque_is_empty(deque)) {
        return NULL;
    }

    deque->tail = deque->tail == 0 ? deque->capacity - 1 : deque->tail - 1;
    const void* value = deque->values[deque->tail];
    deque->values[deque->tail] = NULL;
    deque->size--;
    return value;
}

const void* deque_pop_front(deque_t* deque)
{
    if (deque_is_empty(deque)) {
        return NULL;
    }

    const void* value = deque->values[deque->head];
    deque->values[deque->head] = NULL;
    deque->head = deque->head == deque->capacity - 1 ? 0 : deque->head + 1;
    deque->size--;
    return value;
}

const void* deque_back(const deque_t* deque)
{
    if (deque_is_empty(deque)) {
        return NULL;
    }

    size_t pos = deque->tail == 0 ? deque->capacity - 1 : deque->tail - 1;
    return deque->values[pos];
}

const void* deque_front(const deque_t* deque)
{
    if (deque_is_empty(deque)) {
        return NULL;
    }

    return deque->values[deque->head];
}

int deque_is_empty(const deque_t* deque)
{
    return deque_size(deque) == 0;
}

void deque_clear(deque_t* deque)
{
    if (deque != NULL) {
        if (deque->head > deque->tail) {
            for (size_t i = 0; i < deque->tail; i++) {
                deque->values[i] = NULL;
            }

            for (size_t i = deque->head; i < deque->capacity; i++) {
                deque->values[i] = NULL;
            }
        } else {
            for (size_t i = deque->head; i < deque->tail; i++) {
                deque->values[i] = NULL;
            }
        }

        deque->size = 0;
        deque->head = 0;
        deque->tail = 0;
    }
}

size_t deque_size(const deque_t* deque)
{
    return deque != NULL ? deque->size : 0;
}

size_t deque_capacity(const deque_t* deque)
{
    return deque != NULL ? deque->capacity : 0;
}

void deque_release(deque_t* deque)
{
    if (deque != NULL) {
        free(deque->values);
        free(deque);
    }
}
