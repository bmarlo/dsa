#include "marlo/hash_set.h"
#include "marlo/hash_table.h"

#include <stdlib.h>

struct hash_set_t {
    hash_table_t* values;
};

hash_set_t* hash_set_new(size_t capacity)
{
    hash_set_t* set = (hash_set_t*) malloc(sizeof(hash_set_t));
    if (set == NULL) {
        return NULL;
    }

    hash_table_t* values = hash_table_new(capacity, HASH_TABLE_ADDRESS);
    if (values == NULL) {
        free(set);
        return NULL;
    }

    set->values = values;
    return set;
}

int hash_set_push(hash_set_t* set, const void* value)
{
    if (set == NULL) {
        return -1;
    }

    return hash_table_push(set->values, value, value);
}

int hash_set_is_empty(const hash_set_t* set)
{
    return hash_set_size(set) == 0;
}

int hash_set_contains(const hash_set_t* set, const void* value)
{
    return set != NULL && hash_table_contains(set->values, value);
}

hash_set_iterator_t hash_set_begin(const hash_set_t* set)
{
    hash_set_iterator_t iter;
    iter.set = NULL;
    iter.pos = 0;
    iter.node = NULL;

    if (hash_set_size(set) > 0) {
        hash_table_iterator_t inner = hash_table_begin(set->values);
        iter.set = set;
        iter.pos = inner.pos;
        iter.node = inner.node;
    }

    return iter;
}

int hash_set_is_valid(hash_set_iterator_t iter)
{
    return iter.node != NULL;
}

hash_set_iterator_t hash_set_next(hash_set_iterator_t iter)
{
    hash_set_iterator_t next;
    next.set = NULL;
    next.pos = 0;
    next.node = NULL;

    if (iter.set != NULL) {
        hash_table_iterator_t inner;
        inner.table = iter.set->values;
        inner.pos = iter.pos;
        inner.node = iter.node;

        inner = hash_table_next(inner);
        if (hash_table_is_valid(inner)) {
            next.set = iter.set;
            next.pos = inner.pos;
            next.node = inner.node;
        }
    }

    return next;
}

const void* hash_set_value(hash_set_iterator_t iter)
{
    hash_table_iterator_t inner;
    inner.table = iter.set != NULL ? iter.set->values : NULL;
    inner.pos = iter.pos;
    inner.node = iter.node;

    hash_table_item_t item = hash_table_item(inner);
    return item.value;
}

void hash_set_remove(hash_set_t* set, const void* value)
{
    if (set != NULL) {
        hash_table_remove(set->values, value);
    }
}

void hash_set_clear(hash_set_t* set)
{
    if (set != NULL) {
        hash_table_clear(set->values);
    }
}

size_t hash_set_size(const hash_set_t* set)
{
    return set != NULL ? hash_table_size(set->values) : 0;
}

size_t hash_set_capacity(const hash_set_t* set)
{
    return set != NULL ? hash_table_capacity(set->values) : 0;
}

float hash_set_load_factor(const hash_set_t* set)
{
    return set != NULL ? hash_table_load_factor(set->values) : 0;
}

void hash_set_release(hash_set_t* set)
{
    if (set != NULL) {
        hash_table_release(set->values);
        free(set);
    }
}
