#pragma once
#include <stdbool.h>

extern int last_x, last_y, curr_x, curr_y, escaped_y;
extern const int n_x, n_y, offset_x, offset_y;

typedef enum cell_contains { EMPTY, ROCK, SAND } CELL;
extern CELL **field;
extern bool pen_is_up;

#define pen_is_down (!pen_is_up)
void draw(void);
void print_field(void);
int drop_sand(bool final_layer);