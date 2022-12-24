#include "structs.h"

int abs(int x) {return x > 0 ? x : -x;}
int manhattan_distance(int x1, int x2, int y1, int y2) {return abs(x2-x1) + abs(y2-y1);}
int mod(int a, int b) { return (a%b + b)%b; }
int min(int x, int y) { return x < y ? x : y; }
int max(int x, int y) { return x > y ? x : y; }

BLIZZARD_LIST_ITEM *add_blizzard(int row, int col, DIRECTION d, BLIZZARD_LIST *l)
{
    BLIZZARD    *b = malloc(sizeof(BLIZZARD));
    b->row = row;
    b->col = col;
    b->d = d;

    BLIZZARD_LIST_ITEM *bli = malloc(sizeof(BLIZZARD_LIST_ITEM)), *this;
    bli->b = b;
    bli->next = NULL;

    this = l->head;

    if(!this) l->head = bli;
    else {
        while(this->next) this = this->next;
        this->next = bli;
    }
    return bli;
}