#include "structs.h"
#include <stdlib.h>
#include <stdio.h>

int yylex(void);

LINKED_LIST list;
LIST_INDEX_ITEM *list_index;

int main(int argc, char **argv)
{

    list.head = NULL;
    list.tail = NULL;
    list.item_count = 0;

    yylex();

    list_index = calloc(list.item_count,sizeof(LIST_INDEX_ITEM));

    LINKED_LIST_ITEM *current = list.head;

    for(int i = 0; i < list.item_count; i++)
    {
        list_index[i].item = current->item;
        list_index[i].location = current;
        list_index[i].ordinal = current->ordinal_position;
        current = current->next;
    }

    for(int i = 0; i < list.item_count; i++) mix(list_index[i].location);

    current = list.head;
    while(current->item != 0) current = current->next;

    int k = 0;
    int answer = 0;

    while(k <= 3000)
    {
        if(k % 1000 == 0 && k != 0) {
            printf("%6d %6d\n",k,current->item);
            answer += current->item;
        }

        k++;
        current = current->next;
    }

    printf("%8d\n",answer);

    // reset the list... for part 2 
    // repair pointers
    for(int i = 0; i < list.item_count; i++)
    {
        if(i==0) {
            list_index[0].location->next = list_index[1].location;
            list_index[0].location->prev = list_index[list.item_count-1].location;
        }
        else if (i == list.item_count - 1) {
            list_index[i].location->prev = list_index[i-1].location;
            list_index[i].location->next = list_index[0].location;
        }
        else {
            list_index[i].location->next = list_index[i+1].location;
            list_index[i].location->prev = list_index[i-1].location;
        }
    }

    for(int i = 0; i < 10; i++){
        current = list.head;
        for(int j = 0; j < list.item_count; j++){
            mix2(list_index[j].location);
        }
    }

    current = list.head;
    while(current->item2 != 0) current = current->next;

    k = 0;
    long long answer2 = 0;

    while(k <= 3000)
    {
        if(k % 1000 == 0 && k != 0) {
            printf("%6d %16lld\n",k,current->item2);
            answer2 += current->item2;
        }

        k++;
        current = current->next;
    }

    printf("%16lld\n",answer2);

    free(list_index);

    return 0;
}