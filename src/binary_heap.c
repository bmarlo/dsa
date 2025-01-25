#include "marlo/binary_heap.h"

#include <stdlib.h>

#define RESIZE_FACTOR 2

struct binary_heap_t {
    const void** values;
    compare_t compare;
    size_t capacity;
    size_t size;
};

binary_heap_t* binary_heap_new(size_t capacity, compare_t compare)
{
    if (compare == NULL) {
        return NULL;
    }

    binary_heap_t* heap = (binary_heap_t*) malloc(sizeof(binary_heap_t));
    if (heap == NULL) {
        return NULL;
    }

    heap->values = NULL;
    heap->compare = compare;
    heap->capacity = capacity;
    heap->size = 0;

    if (capacity > 0) {
        const void** values = (const void**) malloc(capacity * sizeof(void*));
        if (values == NULL) {
            free(heap);
            return NULL;
        }

        for (size_t i = 0; i < capacity; i++) {
            values[i] = NULL;
        }

        heap->values = values;
    }

    return heap;
}

static int binary_heap_resize(binary_heap_t* heap)
{
    size_t new_capacity = heap->capacity > 0 ? heap->capacity * RESIZE_FACTOR : 1;
    const void** new_values = (const void**) malloc(new_capacity * sizeof(void*));
    if (new_values == NULL) {
        return -1;
    }

    for (size_t i = 0; i < heap->size; i++) {
        new_values[i] = heap->values[i];
    }

    for (size_t i = heap->size; i < new_capacity; i++) {
        new_values[i] = NULL;
    }

    free(heap->values);
    heap->values = new_values;
    heap->capacity = new_capacity;
    return 0;
}

static void binary_heap_swap(binary_heap_t* heap, size_t i, size_t j)
{
    const void* value = heap->values[i];
    heap->values[i] = heap->values[j];
    heap->values[j] = value;
}

int binary_heap_push(binary_heap_t* heap, const void* value)
{
    if (heap == NULL || value == NULL) {
        return -1;
    }

    if (heap->size == heap->capacity) {
        int error = binary_heap_resize(heap);
        if (error == -1) {
            return -1;
        }
    }

    heap->values[heap->size] = value;
    size_t i = heap->size;
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (heap->compare(heap->values[parent], heap->values[i]) < 0) {
            break;
        }

        binary_heap_swap(heap, parent, i);
        i = parent;
    }

    heap->size++;
    return 0;
}

const void* binary_heap_pop(binary_heap_t* heap)
{
    if (binary_heap_is_empty(heap)) {
        return NULL;
    }

    const void* value = heap->values[0];
    heap->values[0] = heap->values[heap->size - 1];
    heap->values[heap->size - 1] = NULL;
    heap->size--;

    size_t i = 0;
    while (i < heap->size) {
        size_t left = 2 * i + 1;
        if (!(left < heap->size)) {
            break;
        }

        size_t right = left + 1;
        size_t child = !(right < heap->size) || heap->compare(heap->values[left], heap->values[right]) < 0 ? left : right;
        if (heap->compare(heap->values[i], heap->values[child]) < 0) {
            break;
        }

        binary_heap_swap(heap, i, child);
        i = child;
    }

    return value;
}

const void* binary_heap_peek(const binary_heap_t* heap)
{
    if (binary_heap_is_empty(heap)) {
        return NULL;
    }

    return heap->values[0];
}

int binary_heap_is_empty(const binary_heap_t* heap)
{
    return binary_heap_size(heap) == 0;
}

void binary_heap_clear(binary_heap_t* heap)
{
    if (heap != NULL) {
        for (size_t i = 0; i < heap->size; i++) {
            heap->values[i] = NULL;
        }

        heap->size = 0;
    }
}

size_t binary_heap_size(const binary_heap_t* heap)
{
    return heap != NULL ? heap->size : 0;
}

size_t binary_heap_capacity(const binary_heap_t* heap)
{
    return heap != NULL ? heap->capacity : 0;
}

void binary_heap_release(binary_heap_t* heap)
{
    if (heap != NULL) {
        free(heap->values);
        free(heap);
    }
}
