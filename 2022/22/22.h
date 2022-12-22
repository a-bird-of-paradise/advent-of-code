#pragma once

typedef enum facing_e { RIGHT, DOWN, LEFT, UP } FACING;
typedef struct position_t { int x; int y; FACING f; } POSITION;

typedef struct node_t { 
    int x;
    int y;
    char value;
    struct node_t *left;
    struct node_t *right;
    struct node_t *up;
    struct node_t *down;
} NODE;