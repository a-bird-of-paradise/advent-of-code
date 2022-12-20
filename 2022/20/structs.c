#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

LINKED_LIST_ITEM *add(const int item)
{
    LINKED_LIST_ITEM *new = malloc(sizeof(LINKED_LIST_ITEM));

    new->item = item;
    new->ordinal_position = INT16_MIN;
    new->next = NULL;
    new->prev = list.tail;

    // is first item? 

    if(!list.head)
    {
        list.head = new;
    } else {
        list.tail->next = new;
    }

    list.tail = new;
    new->ordinal_position = list.item_count;
    list.item_count++;

    new->item2 = 811589153LL * (new->item);

    return new;
}

void mix(LINKED_LIST_ITEM *x)
{
    int counter = x->item;

    if(x->item > 0) counter = (x->item) % (list.item_count-1);
    if(x->item ==0) counter = 0;
    if(x->item < 0) counter =-( (-x->item) % (list.item_count-1));

    LINKED_LIST_ITEM *left, *right, *left_left, *right_right;
    if(counter > 0){
        while(counter > 0)
        {
            left = x->prev;
            right = x->next;
            right_right = right->next;

            left->next = right;
            right->prev = left;

            right->next = x;
            x->prev = right;

            x->next = right_right;
            right_right->prev = x;

            counter--;
        }
    } else if(counter < 0) {
        while(counter < 0)
        {
            left = x->prev;
            right = x->next;
            left_left = left->prev;

            left->next = right;
            right->prev = left;

            left->prev = x;
            x->next = left;

            x->prev = left_left;
            left_left->next = x;

            counter++;
        }
    }
}

void mix2(LINKED_LIST_ITEM *x)
{
    long long counter;

    if(x->item2 > 0) counter = (x->item2) % (list.item_count-1);
    if(x->item2 < 0) counter = -( (-x->item2) % (list.item_count-1));
    if(x->item2 ==0) counter = 0;

    LINKED_LIST_ITEM *left, *right, *left_left, *right_right;
    if(counter > 0){
        while(counter > 0)
        {
            left = x->prev;
            right = x->next;
            right_right = right->next;

            left->next = right;
            right->prev = left;

            right->next = x;
            x->prev = right;

            x->next = right_right;
            right_right->prev = x;

            counter--;
        }
    } else if(counter < 0) {
        while(counter < 0)
        {
            left = x->prev;
            right = x->next;
            left_left = left->prev;

            left->next = right;
            right->prev = left;

            left->prev = x;
            x->next = left;

            x->prev = left_left;
            left_left->next = x;

            counter++;
        }
    }
}