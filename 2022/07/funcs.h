#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct node {
    char *name;
    unsigned long long size;
    struct node *parent;
    struct node *children;
    struct node *next;
    bool is_directory;
};

typedef struct node NODE;

extern NODE *current_dir, *root_dir;

void init(void);
NODE *find_child(NODE *base, char *name);
void add_child(NODE *base, char *name, unsigned long long size, bool is_directory);
void print_tree();
void set_node_sizes(void);
void sum_below(const unsigned long long x);
void smallest_suitable_dir(const unsigned long long disk_size, const unsigned long long needed);