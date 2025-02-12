#include "marlo/vector.h"

#include <stdlib.h>

#define RESIZE_FACTOR 2

struct vector_t {
    const void** values;
    size_t capacity;
    size_t size;
};

vector_t* vector_new(size_t capacity)
{
    vector_t* vector = (vector_t*) malloc(sizeof(vector_t));
    if (vector == NULL) {
        return NULL;
    }

    vector->values = NULL;
    vector->capacity = capacity;
    vector->size = 0;

    if (capacity > 0) {
        const void** values = (const void**) malloc(capacity * sizeof(void*));
        if (values == NULL) {
            free(vector);
            return NULL;
        }

        for (size_t i = 0; i < capacity; i++) {
            values[i] = NULL;
        }

        vector->values = values;
    }

    return vector;
}

static int vector_resize(vector_t* vector)
{
    size_t new_capacity = vector->capacity > 0 ? vector->capacity * RESIZE_FACTOR : 1;
    const void** new_values = (const void**) malloc(new_capacity * sizeof(void*));
    if (new_values == NULL) {
        return -1;
    }

    for (size_t i = 0; i < vector->size; i++) {
        new_values[i] = vector->values[i];
    }

    for (size_t i = vector->size; i < new_capacity; i++) {
        new_values[i] = NULL;
    }

    free(vector->values);
    vector->values = new_values;
    vector->capacity = new_capacity;
    return 0;
}

int vector_push(vector_t* vector, const void* value)
{
    if (vector == NULL || value == NULL) {
        return -1;
    }

    if (vector->size == vector->capacity) {
        int error = vector_resize(vector);
        if (error == -1) {
            return -1;
        }
    }

    vector->values[vector->size] = value;
    vector->size++;
    return 0;
}

const void* vector_pop(vector_t* vector)
{
    if (vector_is_empty(vector)) {
        return NULL;
    }

    const void* value = vector->values[vector->size - 1];
    vector->values[vector->size - 1] = NULL;
    vector->size--;
    return value;
}

const void* vector_at(const vector_t* vector, size_t pos)
{
    if (vector != NULL && pos < vector->size) {
        return vector->values[pos];
    }

    return NULL;
}

int vector_foreach(const vector_t* vector, callback_t on_value)
{
    if (vector == NULL || on_value == NULL) {
        return -1;
    }

    for (size_t i = 0; i < vector->size; i++) {
        on_value(vector->values[i]);
    }

    return 0;
}

int vector_is_empty(const vector_t* vector)
{
    return vector_size(vector) == 0;
}

void vector_remove(vector_t* vector, size_t pos)
{
    if (vector != NULL && pos < vector->size) {
        for (size_t i = pos + 1; i < vector->size; i++) {
            vector->values[i - 1] = vector->values[i];
        }

        vector->values[vector->size - 1] = NULL;
        vector->size--;
    }
}

void vector_clear(vector_t* vector)
{
    if (vector != NULL) {
        for (size_t i = 0; i < vector->size; i++) {
            vector->values[i] = NULL;
        }

        vector->size = 0;
    }
}

size_t vector_size(const vector_t* vector)
{
    return vector != NULL ? vector->size : 0;
}

size_t vector_capacity(const vector_t* vector)
{
    return vector != NULL ? vector->capacity : 0;
}

static void vector_swap(vector_t* vector, size_t i, size_t j)
{
    const void* value = vector->values[i];
    vector->values[i] = vector->values[j];
    vector->values[j] = value;
}

static size_t vector_partition(vector_t* vector, size_t i, size_t j, compare_t compare)
{
    i--;
    size_t pivot = j;
    for (size_t k = i + 1; k < j; k++) {
        if (compare(vector->values[k], vector->values[pivot]) < 0) {
            vector_swap(vector, ++i, k);
        }
    }

    vector_swap(vector, i + 1, j);
    return i + 1;
}

static void vector_quicksort(vector_t* vector, size_t i, size_t j, compare_t compare)
{
    if (i < j) {
        size_t pivot = vector_partition(vector, i, j, compare);
        vector_quicksort(vector, i, pivot - 1, compare);
        vector_quicksort(vector, pivot + 1, j, compare);
    }
}

int vector_sort(vector_t* vector, compare_t compare)
{
    if (vector == NULL || compare == NULL) {
        return -1;
    }

    if (vector->size > 1) {
        vector_quicksort(vector, 0, vector->size - 1, compare);
    }

    return 0;
}

void vector_release(vector_t* vector)
{
    if (vector != NULL) {
        free(vector->values);
        free(vector);
    }
}
