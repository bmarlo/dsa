#include "marlo/hash_table.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOAD_FACTOR 0.75

typedef struct list_node_t list_node_t;

struct list_node_t {
    hash_table_item_t item;
    list_node_t* next;
};

typedef struct linked_list_t {
    list_node_t* head;
    size_t size;
} linked_list_t;

struct hash_table_t {
    int mode;
    linked_list_t** items;
    size_t capacity;
    size_t size;
};

hash_table_t* hash_table_new(size_t capacity, int mode)
{
    if (mode != HASH_TABLE_ADDRESS && mode != HASH_TABLE_STRING) {
        return NULL;
    }

    hash_table_t* table = (hash_table_t*) malloc(sizeof(hash_table_t));
    if (table == NULL) {
        return NULL;
    }

    table->mode = mode;
    table->items = NULL;
    table->capacity = capacity;
    table->size = 0;

    if (capacity > 0) {
        linked_list_t** items = (linked_list_t**) malloc(capacity * sizeof(linked_list_t*));
        if (items == NULL) {
            free(table);
            return NULL;
        }

        for (size_t i = 0; i < capacity; i++) {
            items[i] = NULL;
        }

        table->items = items;
    }

    return table;
}

int hash_table_mode(const hash_table_t* table)
{
    if (table == NULL) {
        return -1;
    }

    return table->mode;
}

int hash_table_is_address(const hash_table_t* table)
{
    return hash_table_mode(table) == HASH_TABLE_ADDRESS;
}

int hash_table_is_string(const hash_table_t* table)
{
    return hash_table_mode(table) == HASH_TABLE_STRING;
}

/**
 * Fowler–Noll–Vo hash.
 * Yeah, science!!1
 */
static size_t hash_impl(int mode, size_t capacity, const void* key)
{
    uint64_t pos = 0xcbf29ce484222325;
    if (mode == HASH_TABLE_ADDRESS) {
        uintptr_t address = (uintptr_t) key;
        for (size_t i = 0; i < sizeof(uintptr_t); i++) {
            uint8_t byte = (address >> (sizeof(uintptr_t) - 1 - i) * 8) & 0xff;
            pos *= 0x100000001b3;
            pos ^= byte;
        }
    } else {
        const char* str = (const char*) key;
        while (*str != '\0') {
            uint8_t byte = *str++;
            pos *= 0x100000001b3;
            pos ^= byte;
        }
    }

    pos %= capacity;
    return (size_t) pos;
}

static int hash_table_rehash(hash_table_t* table)
{
    size_t new_capacity = table->capacity > 0 ? table->capacity * 2 : 1;
    linked_list_t** new_items = (linked_list_t**) malloc(new_capacity * sizeof(linked_list_t*));
    if (new_items == NULL) {
        return -1;
    }

    for (size_t i = 0; i < new_capacity; i++) {
        new_items[i] = NULL;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        linked_list_t* list = table->items[i];
        if (list != NULL) {
            list_node_t* iter = list->head;
            while (iter != NULL) {
                list_node_t* next = iter->next;
                const void* key = iter->item.key;
                const void* value = iter->item.value;

                size_t new_pos = hash_impl(table->mode, new_capacity, key);
                linked_list_t* new_list = new_items[new_pos];
                if (new_list == NULL) {
                    new_list = (linked_list_t*) malloc(sizeof(linked_list_t));
                    if (new_list == NULL) {
                        for (size_t i = 0; i < new_capacity; i++) {
                            linked_list_t* new_list = new_items[i];
                            if (new_list != NULL) {
                                list_node_t* new_iter = new_list->head;
                                while (new_iter != NULL) {
                                    list_node_t* new_next = new_iter->next;
                                    free(new_iter);
                                    new_iter = new_next;
                                }

                                free(new_list);
                            }
                        }

                        free(new_items);
                        return -1;
                    }

                    new_list->head = NULL;
                    new_list->size = 0;
                    new_items[new_pos] = new_list;
                }

                list_node_t* new_tail = NULL;
                list_node_t* new_iter = new_list->head;
                while (new_iter != NULL) {
                    new_tail = new_iter;
                    new_iter = new_iter->next;
                }

                list_node_t* new_node = (list_node_t*) malloc(sizeof(list_node_t));
                if (new_node == NULL) {
                    for (size_t i = 0; i < new_capacity; i++) {
                        linked_list_t* new_list = new_items[i];
                        if (new_list != NULL) {
                            list_node_t* new_iter = new_list->head;
                            while (new_iter != NULL) {
                                list_node_t* new_next = new_iter->next;
                                free(new_iter);
                                new_iter = new_next;
                            }

                            free(new_list);
                        }
                    }

                    free(new_items);
                    return -1;
                }

                new_node->item.key = key;
                new_node->item.value = value;
                new_node->next = NULL;

                if (new_tail != NULL) {
                    new_tail->next = new_node;
                }

                if (new_list->head == NULL) {
                    new_list->head = new_node;
                }

                new_list->size++;
                iter = next;
            }
        }
    }

    for (size_t i = 0; i < table->capacity; i++) {
        linked_list_t* list = table->items[i];
        if (list != NULL) {
            list_node_t* iter = list->head;
            while (iter != NULL) {
                list_node_t* next = iter->next;
                free(iter);
                iter = next;
            }

            free(list);
        }
    }

    free(table->items);
    table->items = new_items;
    table->capacity = new_capacity;
    return 0;
}

static size_t hash_table_hash(const hash_table_t* table, const void* key)
{
    return hash_impl(table->mode, table->capacity, key);
}

static int hash_table_equals(const hash_table_t* table, const void* key, const void* target)
{
    if (table->mode == HASH_TABLE_ADDRESS) {
        return key == target;
    }

    return !strcmp((const char*) key, (const char*) target);
}

int hash_table_push(hash_table_t* table, const void* key, const void* value)
{
    if (table == NULL || value == NULL) {
        return -1;
    }

    if (!hash_table_capacity(table) || hash_table_load_factor(table) > MAX_LOAD_FACTOR) {
        int error = hash_table_rehash(table);
        if (error == -1) {
            return -1;
        }
    }

    size_t pos = hash_table_hash(table, key);
    linked_list_t* list = table->items[pos];
    if (list == NULL) {
        list = (linked_list_t*) malloc(sizeof(linked_list_t));
        if (list == NULL) {
            return -1;
        }

        list->head = NULL;
        list->size = 0;
        table->items[pos] = list;
    }

    list_node_t* tail = NULL;
    list_node_t* iter = list->head;
    while (iter != NULL) {
        if (hash_table_equals(table, iter->item.key, key)) {
            iter->item.value = value;
            return 0;
        }

        tail = iter;
        iter = iter->next;
    }

    list_node_t* node = (list_node_t*) malloc(sizeof(list_node_t));
    if (node == NULL) {
        return -1;
    }

    node->item.key = key;
    node->item.value = value;
    node->next = NULL;

    if (tail != NULL) {
        tail->next = node;
    }

    if (list->head == NULL) {
        list->head = node;
    }

    list->size++;
    table->size++;
    return 0;
}

const void* hash_table_at(const hash_table_t* table, const void* key)
{
    if (!hash_table_capacity(table)) {
        return NULL;
    }

    size_t pos = hash_table_hash(table, key);
    linked_list_t* list = table->items[pos];
    if (list != NULL) {
        list_node_t* iter = list->head;
        while (iter != NULL) {
            if (hash_table_equals(table, iter->item.key, key)) {
                return iter->item.value;
            }
            iter = iter->next;
        }
    }

    return NULL;
}

int hash_table_is_empty(const hash_table_t* table)
{
    return hash_table_size(table) == 0;
}

int hash_table_contains(const hash_table_t* table, const void* key)
{
    return hash_table_at(table, key) != NULL;
}

hash_table_iterator_t hash_table_begin(const hash_table_t* table)
{
    hash_table_iterator_t iter;
    iter.table = NULL;
    iter.pos = 0;
    iter.node = NULL;

    if (hash_table_size(table) > 0) {
        iter.table = table;
        for (size_t i = 0; i < table->capacity; i++) {
            linked_list_t* list = table->items[i];
            if (list != NULL && list->head != NULL) {
                iter.pos = i;
                iter.node = list->head;
                break;
            }
        }
    }

    return iter;
}

int hash_table_is_valid(hash_table_iterator_t iter)
{
    return iter.node != NULL;
}

hash_table_iterator_t hash_table_next(hash_table_iterator_t iter)
{
    hash_table_iterator_t next;
    next.table = NULL;
    next.pos = 0;
    next.node = NULL;

    if (iter.table != NULL) {
        const list_node_t* node = (const list_node_t*) iter.node;
        if (node != NULL) {
            if (node->next != NULL) {
                next.table = iter.table;
                next.pos = iter.pos;
                next.node = node->next;
                return next;
            }
        }

        for (size_t i = iter.pos + 1; i < iter.table->capacity; i++) {
            linked_list_t* list = iter.table->items[i];
            if (list != NULL && list->head != NULL) {
                next.table = iter.table;
                next.pos = i;
                next.node = list->head;
                return next;
            }
        }
    }

    return next;
}

hash_table_item_t hash_table_item(hash_table_iterator_t iter)
{
    hash_table_item_t item;
    item.key = iter.node != NULL ? ((const list_node_t*) iter.node)->item.key : NULL;
    item.value = iter.node != NULL ? ((const list_node_t*) iter.node)->item.value : NULL;
    return item;
}

int hash_table_remove(hash_table_t* table, const void* key)
{
    if (!hash_table_capacity(table)) {
        return -1;
    }

    size_t pos = hash_table_hash(table, key);
    linked_list_t* list = table->items[pos];
    if (list == NULL) {
        return -1;
    }

    list_node_t* prev = NULL;
    list_node_t* iter = list->head;
    while (iter != NULL) {
        list_node_t* next = iter->next;
        if (hash_table_equals(table, iter->item.key, key)) {
            if (prev != NULL) {
                prev->next = next;
            }

            if (iter == list->head) {
                list->head = next;
            }

            free(iter);
            list->size--;
            table->size--;
            return 0;
        }

        prev = iter;
        iter = next;
    }

    return -1;
}

void hash_table_clear(hash_table_t* table)
{
    if (table != NULL) {
        for (size_t i = 0; i < table->capacity; i++) {
            linked_list_t* list = table->items[i];
            if (list != NULL) {
                list_node_t* iter = list->head;
                while (iter != NULL) {
                    list_node_t* next = iter->next;
                    free(iter);
                    iter = next;
                }

                list->head = NULL;
                list->size = 0;
            }
        }

        table->size = 0;
    }
}

size_t hash_table_size(const hash_table_t* table)
{
    return table != NULL ? table->size : 0;
}

size_t hash_table_capacity(const hash_table_t* table)
{
    return table != NULL ? table->capacity : 0;
}

float hash_table_load_factor(const hash_table_t* table)
{
    return hash_table_capacity(table) > 0 ? (float) table->size / (float) table->capacity : 0;
}

void hash_table_release(hash_table_t* table)
{
    if (table != NULL) {
        for (size_t i = 0; i < table->capacity; i++) {
            linked_list_t* list = table->items[i];
            if (list != NULL) {
                list_node_t* iter = list->head;
                while (iter != NULL) {
                    list_node_t* next = iter->next;
                    free(iter);
                    iter = next;
                }

                free(list);
            }
        }

        free(table->items);
        free(table);
    }
}
