#pragma once

typedef struct linked_list_item_t {
    int item;
    int ordinal_position;
    long long item2;
    struct linked_list_item_t *prev;
    struct linked_list_item_t *next;
} LINKED_LIST_ITEM;

typedef struct linked_list_t {
    int item_count;
    LINKED_LIST_ITEM *head;
    LINKED_LIST_ITEM *tail;
} LINKED_LIST;

typedef struct list_index_item_t {
    int item;
    int ordinal;
    long long item2;
    LINKED_LIST_ITEM *location;
} LIST_INDEX_ITEM;

LINKED_LIST_ITEM *add(const int item);

extern LINKED_LIST list;

extern LIST_INDEX_ITEM *list_index;

void mix(LINKED_LIST_ITEM *x);
void mix2(LINKED_LIST_ITEM *x);