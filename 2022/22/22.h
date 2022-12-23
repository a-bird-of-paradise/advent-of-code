#pragma once
#include <stdbool.h>

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
    bool is_edge;
    FACING post_edge_facing;
    FACING which_edge;
} NODE;

extern char field[200][201];
extern NODE nodes[200][200];
int mod (const int a, const int b);

void move_1(void);
void init_part_1(void);
void init_part_2_test(void);

int abs(const int x);
void print_facing(FACING x);
void print_nodes(void);
