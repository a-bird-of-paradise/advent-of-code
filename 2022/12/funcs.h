#pragma once
#include "structs.h"

void visit(NODE *current);
void walk(NODE *towalk);
void reset_nodes(void);
void add_neighbour(NODE *addee, NODE *new_neighbour);
void add_neighbours(void);
