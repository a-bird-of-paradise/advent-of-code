%{
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
int yylex(void);

int yy_pairindex = 0;
int yy_indexcounter = 0;
int compare(NODE *left, NODE *right);
void part2();

%}

%union {    NODE *n;    int i;  }

%token ENDL
%token <i> INT
%type <n> list list_inner line list_item
%type <i> linepair linepairs
%type ending file

%destructor { free_node($$);  } <n>

%%

file:   linepairs                   {   printf("%8d\n",$1); part2();    }

linepairs: linepair                 {   $$ = $1;    }
    | linepairs linepair            {   $$ += $2;   };

linepair: line line ending          {   yy_pairindex++;
                                        $$ = compare($1,$2);
                                        yy_indexcounter += $$;
                                        add_node_to_list(&list_of_nodes,$1);
                                        add_node_to_list(&list_of_nodes,$2);    };
line: list ENDL;

list:   '[' list_inner ']'          {   $$ = new_node($2,NULL,LIST);    };

list_inner:                         {   $$ = NULL;                  }
    |   list_item ',' list_inner    {   $1->right = $3; $$ = $1;    }
    |   list_item                   {   $$ = $1;                    };

list_item:
        INT     {   $$ = new_num($1);   }
    |   list    {   $$ = $1;            };

ending: ENDL | YYEOF;

%%

typedef enum order { CORRECT, MAYBE, INCORRECT } ORDER;

void promote_to_list(NODE *to_promote)
{
    if(to_promote->left) { yyerror("Numbers should not have children\n"); exit(1);    }

    to_promote->left = new_num(to_promote->payload); 
    to_promote->right = to_promote->right; // unchanged
    to_promote->payload = 0;
    to_promote->type = LIST;
}

void demote_to_number(NODE *to_demote)
{
    to_demote->payload = to_demote->left->payload;
    to_demote->type = NUM;
    to_demote->right = to_demote->right;
    if(to_demote->left) free(to_demote->left);
    to_demote->left = NULL;
}

ORDER compare_ints(NODE *left, NODE *right)
{
    ORDER answer = MAYBE;
    if(left->payload == right->payload) answer = MAYBE;
    else if(left->payload < right->payload) answer = CORRECT; 
    else answer = INCORRECT;
    return answer; 
}

ORDER Ordering(NODE *left, NODE *right)
{
    ORDER answer = MAYBE;

    if(left->type == NUM && right->type == NUM)
        return compare_ints(left,right);

    if(left->type == NUM && right->type == LIST) {
        promote_to_list(left);
        answer = Ordering(left,right);
        demote_to_number(left);
        return answer;
    }

    if(left->type == LIST && right->type == NUM) {
        promote_to_list(right);
        answer = Ordering(left,right);
        demote_to_number(right);
        return answer;
    }

    // both lists; iterate over children until something goes wrong

    if(!left->left && right->left) { // left empty, right isn't == OK
        return CORRECT;
    }

    if(left->left && !right->left) { // left full, right empty == problem
        return INCORRECT;
    }

    if(!left->left && !right->left) { // both empty
        return MAYBE;
    }

    NODE *left_child = left->left, *right_child = right->left;
    while(answer == MAYBE)
    {
        answer = Ordering(left_child,right_child);

        left_child = left_child->right;
        right_child = right_child->right;
        // did we run out of a list?
        if(answer != MAYBE) break;
        if(!left_child && right_child) answer = CORRECT;
        if(left_child && !right_child) answer = INCORRECT;
        if(!left_child && !right_child) {answer = MAYBE; break;}
    }

    return answer;

}

int compare(NODE *left, NODE *right)
{
    int answer_ = Ordering(left,right);
    int answer = answer_ != INCORRECT ? yy_pairindex : 0;
    return answer;
}

int cmp(const void *a, const void *b)
{
    NODE *L = *(NODE**)a, *R = *(NODE**)b;
    ORDER answer = Ordering(L,R);

    if(answer == CORRECT) return -1;
    if(answer == INCORRECT) return 1;
    return 0;
}

void part2(void)
{
    int length = 0, answer = 1;
    NODELISTITEM *current = list_of_nodes.head;

    NODE *sentinel1, *sentinel2;

    while(current)
    {
        length++;
        current = current->next;
    }

    NODE **arr = malloc(length * sizeof(NODE *));

    current = list_of_nodes.head;

    for(int i = 0; i < length; i++){
        arr[i] = current->the_node;
        current=current->next;
    }

    sentinel1 = arr[length-1];
    sentinel2 = arr[length-2];

    qsort(arr,length,sizeof(NODE *),&cmp);

    for(int i = 0; i < length; i++)
    {
        if(arr[i] == sentinel1 || arr[i] == sentinel2) answer *= (i+1);
    }

    printf("%8d\n",answer);

    free(arr);

}