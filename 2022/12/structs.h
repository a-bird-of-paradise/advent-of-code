#pragma once
#define MAXROWS 100
#include <stdbool.h>

typedef struct node {
    bool visited;
    unsigned distance;
    struct node_list_item *first_neighbour;
}   NODE;

typedef struct node_list_item {
    struct node_list_item *next;
    NODE    *this_node;
}   LISTITEM;

typedef struct node_list {
    LISTITEM *head;
}   NODELIST;
