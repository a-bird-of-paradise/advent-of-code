#pragma once
#include "structs.h"

typedef enum order { CORRECT, MAYBE, INCORRECT } ORDER;
extern int yy_pairindex;
void promote_to_list(NODE *to_promote);
void demote_to_number(NODE *to_demote);

ORDER compare_ints(NODE *left, NODE *right);
ORDER Ordering(NODE *left, NODE *right);
int compare(NODE *left, NODE *right);
int cmp(const void *a, const void *b);
void part2(void);
