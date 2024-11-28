#include "marlo/hash_table.h"

#include <stdint.h>
#include <stdlib.h>

#define MAX_LOAD_FACTOR 0.75

typedef struct list_node_t list_node_t;

struct list_node_t {
    const void* key;
    void* value;
    list_node_t* next;
};

typedef struct linked_list_t {
    list_node_t* head;
    size_t size;
} linked_list_t;

struct hash_table_t {
    linked_list_t** items;
    size_t capacity;
    size_t size;
};

hash_table_t* hash_table_new(size_t capacity)
{
    hash_table_t* table = (hash_table_t*) malloc(sizeof(hash_table_t));
    if (table == NULL) {
        return NULL;
    }

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

/**
 * Fowler–Noll–Vo hash.
 * Yeah, science!!1
 */
static size_t hash_impl(size_t capacity, const void* key)
{
    uint64_t pos = 0xcbf29ce484222325;
    uintptr_t address = (uintptr_t) key;
    for (size_t i = 0; i < sizeof(uintptr_t); i++) {
        uint8_t byte = (address >> (sizeof(uintptr_t) - 1 - i) * 8) & 0xff;
        pos *= 0x100000001b3;
        pos ^= byte;
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
                const void* key = iter->key;
                void* value = iter->value;

                size_t new_pos = hash_impl(new_capacity, key);
                linked_list_t* new_list = new_items[new_pos];
                if (new_list == NULL) {
                    new_list = (linked_list_t*) malloc(sizeof(linked_list_t));
                    if (new_list == NULL) {
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

                new_node->key = key;
                new_node->value = value;
                new_node->next = NULL;

                if (new_tail != NULL) {
                    new_tail->next = new_node;
                }

                if (new_list->head == NULL) {
                    new_list->head = new_node;
                }

                new_list->size++;
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
    return hash_impl(table->capacity, key);
}

int hash_table_push(hash_table_t* table, const void* key, void* value)
{
    if (table == NULL) {
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
        if (iter->key == key) {
            iter->value = value;
            break;
        }

        tail = iter;
        iter = iter->next;
    }

    if (iter == NULL) {
        list_node_t* node = (list_node_t*) malloc(sizeof(list_node_t));
        if (node == NULL) {
            return -1;
        }

        node->key = key;
        node->value = value;
        node->next = NULL;

        if (tail != NULL) {
            tail->next = node;
        }

        if (list->head == NULL) {
            list->head = node;
        }

        list->size++;
    }

    table->size++;
    return 0;
}

void* hash_table_at(const hash_table_t* table, const void* key)
{
    if (!hash_table_capacity(table)) {
        return NULL;
    }

    size_t pos = hash_table_hash(table, key);
    linked_list_t* list = table->items[pos];
    if (list != NULL) {
        list_node_t* iter = list->head;
        while (iter != NULL) {
            if (iter->key == key) {
                return iter->value;
            }
            iter = iter->next;
        }
    }

    return NULL;
}

int hash_table_contains(const hash_table_t* table, const void* key)
{
    return hash_table_at(table, key) != NULL;
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
        if (iter->key == key) {
            if (prev != NULL) {
                prev->next = next;
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
