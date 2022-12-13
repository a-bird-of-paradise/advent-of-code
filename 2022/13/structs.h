#pragma once

typedef enum node_type { LIST = 10, NUM } NODE_TYPE;

typedef struct node {
    struct node *left;
    struct node *right;
    int payload;
    NODE_TYPE type;
} NODE;

typedef struct node_list_item {
    NODE *the_node;
    struct node_list_item *next;
} NODELISTITEM;

typedef struct node_list {
    NODELISTITEM *head;
} NODELIST;

extern NODELIST list_of_nodes;
extern int yylineno; // lexer
void yyerror(char *s, ...); 

NODE *new_node(NODE *left, NODE *right, NODE_TYPE type);
NODE *new_num(int n);

void free_node(NODE *to_free);
void print_node(NODE *to_print);
void flat_print(NODE *to_print);
void add_node_to_list(NODELIST *list, NODE *to_add);
