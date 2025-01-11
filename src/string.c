#include "marlo/string.h"

#include <stdlib.h>
#include <string.h>

#define RESIZE_FACTOR 2

struct string_t {
    char* value;
    size_t capacity;
    size_t size;
};

string_t* string_new(size_t capacity)
{
    capacity++;
    if (capacity == 0) {
        return NULL;
    }

    string_t* string = (string_t*) malloc(sizeof(string_t));
    if (string == NULL) {
        return NULL;
    }

    char* value = (char*) malloc(capacity);
    if (value == NULL) {
        free(string);
        return NULL;
    }

    memset(value, '\0', capacity);
    string->value = value;
    string->capacity = capacity;
    string->size = 0;
    return string;
}

static int string_resize(string_t* string, size_t new_capacity)
{
    char* new_value = (char*) malloc(new_capacity);
    if (new_value == NULL) {
        return -1;
    }

    memcpy(new_value, string->value, string->size);
    memset(&new_value[string->size], '\0', new_capacity - string->size);

    free(string->value);
    string->value = new_value;
    string->capacity = new_capacity;
    return 0;
}

int string_push(string_t* string, char c)
{
    if (string == NULL) {
        return -1;
    }

    if (string->size == string->capacity - 1) {
        int error = string_resize(string, string->capacity * RESIZE_FACTOR);
        if (error == -1) {
            return -1;
        }
    }

    string->value[string->size] = c;
    string->size++;
    return 0;
}

int string_append(string_t* string, const char* str)
{
    if (string == NULL || str == NULL) {
        return -1;
    }

    size_t size = strlen(str);
    size_t new_size = string->size + size;
    if (new_size > string->capacity - 1) {
        size_t new_capacity = string->capacity * RESIZE_FACTOR;
        if (new_size > new_capacity - 1) {
            new_capacity = new_size + 1;
        }

        int error = string_resize(string, new_capacity);
        if (error == -1) {
            return -1;
        }
    }

    memcpy(&string->value[string->size], str, size);
    string->size = new_size;
    return 0;
}

void string_clear(string_t* string)
{
    if (string != NULL) {
        memset(string->value, '\0', string->size);
        string->size = 0;
    }
}

int string_is_empty(const string_t* string)
{
    return string_size(string) == 0;
}

const char* string_value(const string_t* string)
{
    return string != NULL ? string->value : NULL;
}

size_t string_size(const string_t* string)
{
    return string != NULL ? string->size : 0;
}

size_t string_capacity(const string_t* string)
{
    return string != NULL ? string->capacity : 0;
}

void string_release(string_t* string)
{
    if (string != NULL) {
        free(string->value);
        free(string);
    }
}
