#pragma once
#include <stdlib.h>

#define LOC_EMPTY 0
#define LOC_OUT 1
#define LOC_RETURN 2
#define LOC_FINAL 4

typedef enum direction_e { UP = '^', DOWN = 'v', LEFT = '<', RIGHT = '>'} DIRECTION;
typedef struct blizzard_t {
    int row;
    int col;
    DIRECTION d;
}   BLIZZARD;

typedef struct blizzard_list_item_t {
    BLIZZARD            *b;
    struct blizzard_list_item_t  *next;
}   BLIZZARD_LIST_ITEM;

typedef struct blizzard_list_t {
    BLIZZARD_LIST_ITEM  *head;
}   BLIZZARD_LIST;

int manhattan_distance(const int x1, const int x2, const int y1, const int y2);
int abs(const int x);
int mod(int a, int b);
int min(int x, int y);
int max(int x, int y);

BLIZZARD_LIST_ITEM *add_blizzard(int row, int col, DIRECTION d, BLIZZARD_LIST *l);
