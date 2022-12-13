#include "structs.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

NODE *new_node(NODE *child, NODE *sibling, NODE_TYPE type)
{
    NODE *answer = malloc(sizeof(NODE));

    if(!answer) {   yyerror("malloc() failed\n");   exit(0);    }

    answer->type = type;
    answer->left = child;
    answer->right = sibling;
    answer->payload=0;

    return answer;
}

NODE *new_num(int n)
{
    NODE *answer = malloc(sizeof(NODE));

    if(!answer) {   yyerror("malloc() failed\n");   exit(0);    }
    answer->type = NUM;
    answer->left = NULL;
    answer->right = NULL;
    answer->payload = n;

    return answer;
}

void yyerror(char *s, ...)
{
    va_list ap;
    va_start(ap,s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

void free_node(NODE *to_free)
{
    if(!to_free) return;
    if(to_free->left) free_node(to_free->left);
    if(to_free->right) free_node(to_free->right);
    free(to_free);
}

void print_node_inner(NODE *to_print, const int level)
{
    int mylevel = level;

    while(mylevel-->0) printf("|");
    printf("[%p][%p][%p]%4d%12d\n",
        to_print,
        to_print->left,
        to_print->right,
        to_print->type,
        to_print->payload);
    if(to_print->left) print_node_inner(to_print->left, level + 1);
    if(to_print->right) print_node_inner(to_print->right, level + 1);
}

void print_node(NODE *to_print)
{
    int level = 0;
    print_node_inner(to_print, level);
}

void flat_print(NODE *to_print)
{
   NODE *next_child;
   if(!to_print) return;
   if(to_print->type==NUM) printf("%d",to_print->payload);
   else {
    printf("[");
    next_child = to_print->left;
    while(next_child)
    {
        flat_print(next_child);
        next_child=next_child->right;
        if(next_child)printf(",");
    }
    printf("]");
   }
}

void add_node_to_list(NODELIST *list, NODE *to_add)
{
    if(!list) { yyerror("Need a list\n"); exit(1);  }

    NODELISTITEM *new_list_item = malloc(sizeof(NODELISTITEM));
    new_list_item->the_node=to_add;
    new_list_item->next=NULL;

    NODELISTITEM *current = list->head;

    if(!current)
    {
        list->head = new_list_item;
    }
    else
    {
        while(current->next) current = current->next;
        current->next = new_list_item;
    }
}