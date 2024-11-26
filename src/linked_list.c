#include "marlo/linked_list.h"

#include <stdlib.h>

struct list_node_t {
    void* value;
    list_node_t* prev;
    list_node_t* next;
};

struct linked_list_t {
    list_node_t* head;
    list_node_t* tail;
    size_t size;
};

linked_list_t* linked_list_new()
{
    linked_list_t* list = malloc(sizeof(linked_list_t));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

int linked_list_push_back(linked_list_t* list, void* value)
{
    if (list == NULL) {
        return -1;
    }

    list_node_t* node = malloc(sizeof(list_node_t));
    if (node == NULL) {
        return -1;
    }

    node->value = value;
    node->prev = list->tail;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    }

    if (list->tail != NULL) {
        list->tail->next = node;
    }

    list->tail = node;
    list->size++;
    return 0;
}

int linked_list_push_front(linked_list_t* list, void* value)
{
    if (list == NULL) {
        return -1;
    }

    list_node_t* node = malloc(sizeof(list_node_t));
    if (node == NULL) {
        return -1;
    }

    node->value = value;
    node->prev = NULL;
    node->next = list->head;

    if (list->tail == NULL) {
        list->tail = node;
    }

    if (list->head != NULL) {
        list->head->prev = node;
    }

    list->head = node;
    list->size++;
    return 0;
}

const list_node_t* linked_list_back(const linked_list_t* list)
{
    return list != NULL ? list->tail : NULL;
}

const list_node_t* linked_list_front(const linked_list_t* list)
{
    return list != NULL ? list->head : NULL;
}

const list_node_t* linked_list_next(const list_node_t* node)
{
    return node != NULL ? node->next : NULL;
}

const list_node_t* linked_list_previous(const list_node_t* node)
{
    return node != NULL ? node->prev : NULL;
}

void* linked_list_value(const list_node_t* node)
{
    return node != NULL ? node->value : NULL;
}

size_t linked_list_size(const linked_list_t* list)
{
    return list != NULL ? list->size : 0;
}

int linked_list_remove(linked_list_t* list, const void* value)
{
    if (list == NULL) {
        return -1;
    }

    list_node_t* prev = NULL;
    list_node_t* iter = list->head;
    while (iter != NULL) {
        list_node_t* next = iter->next;
        if (iter->value == value) {
            if (prev != NULL) {
                prev->next = next;
            }

            if (next != NULL) {
                next->prev = prev;
            }

            if (iter == list->tail) {
                list->tail = prev;
            }

            if (iter == list->head) {
                list->head = next;
            }

            list->size--;
            free(iter);
            return 0;
        }

        prev = iter;
        iter = next;
    }

    return -1;
}

void linked_list_release(linked_list_t* list)
{
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
